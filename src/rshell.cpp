//MACRO
#define popfront() for(int z = 0; z < static_cast<int>(set.size()) - 1; ++z) \
				   {	\
				        set.at(z) = set.at(z+1); \
				   } 	\
				   set.pop_back();



#define inputcont 		string _command;                                            \
						getline(cin, _command); 									\
						char _instruct[512];			\
	    				for(int i = 0; i < static_cast<int>(_command.size()); i++) 				\
	    				{														\
	        				_instruct[i] = _command.at(i); 						\
	    				}														\
						_instruct[_command.size()] = '\0';						\
						char* _token = strtok((char*)_instruct, " ");				\
						cmds.push_back(_token);									\
				    	while(_token != NULL)									\
				    	{														\
				    		 _token = strtok(NULL, " ");							\
				    		 if(_token == NULL)									\
				    		 {													\
				        		 break;											\
				        	 }													\
				        cmds.push_back(_token);								\
				    	}

#include <iostream>
#include <vector>
#include <string>

//fork, execvp, wait
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

//Global Variables
const unsigned buffSize = BUFSIZ;

//rshell prompt
void initializeUser()
{
	char* user = getlogin();
	char host[buffSize];
	int hostID = gethostname(host, 1024);
	//Guards to be added for NULL cases
	//Host to be added
	if(user == NULL)
	{
		//perror("Unidentified User");
	}
	else if(hostID == -1)
	{
		//perror("Unidentified Host");
	}
	else
	{
		cout << user << '@' << host << "$ ";
	}
}

//rshell
bool rshell(vector<string>& set);


int main()
{
	//rshell
	
    //fork
    while(1)
    {
		string command;		
		
		
		//rshell prompt
    	initializeUser();
    	
		//retrieve line
		getline(cin, command);
	
		char instruct[512];
		
	    // Load command into instruct
	    for(int i = 0; i < static_cast<int>(command.size()); i++)
	    {
	        instruct[i] = command.at(i);
	    }
	    
		//NULL
		instruct[command.size()] = '\0';
		//first Initalization
		vector<string> cmds;
		
		char* token = strtok((char*)instruct, " ");
		
		//Push main command
		cmds.push_back(token);
		
		//Insert arguments / cmds / connectors
	    while(token != NULL)
	    {
	        token = strtok(NULL, " ");
	        
	        //NULL Guard
	        if(token == NULL)
	        {
	            break;
	        }
	        
	        cmds.push_back(token);
	    }
	    
	    while (cmds.at(cmds.size() - 1) == "&&" || cmds.at(cmds.size() - 1) == "||") 
	    	{
	    		cout << "> ";
	    		
	    		inputcont;
	    	}
	    
	    //By now all are parsed by space
	    
	    //boolean for determining successive operations
	    bool status = true;
	    
	    //actual child-progress command storage
	    vector<string> set;
	    int j = 0;
	    
	    //Base case command
	    for(; j < static_cast<int>(cmds.size()); ++j) {
	    	
	    	if ((cmds.at(j) != "||") &&
	    		(cmds.at(j) != "&&") &&
	    		(cmds.at(j) != ";")  &&
	    		(cmds.at(j) != "#")    ) 
	    		{
	    			set.push_back(cmds.at(j));
	    		}
	    		else
	    		{	
	    			break;
	    		}
	    }
	    
	    status = rshell(set);
	    set.clear();
	    
	    //loading in successive commands after base case
	    for(int i = j; i < static_cast<int>(cmds.size()); i++)
	    {
	    	
	    	set.push_back(cmds.at(i));
	    	
	    	if (set.size() > 1 && (cmds.at(i) == "||" || 
	    						   cmds.at(i) == "&&" || 
	    						   cmds.at(i) == ";"  ||
	    						   cmds.at(i) == "#")) 
	    	{
			    if(set.at(0) == ";")
			    {
			    	--i;
			    	set.pop_back();
			    	popfront();
			    	
			    	status = rshell(set);
			    	set.clear();
			    }	
			    else if(set.at(0) == "&&")
			    {
			    	--i;
			    	set.pop_back();
			    	popfront();
			    	
			    	if(status == true)
			    	{
			    		status = rshell(set);
			    	}
			    	set.clear();
			    }
			    else if(set.at(0) == "||")
			    {
			    	--i;
			    	set.pop_back();
			    	popfront();
			    	
			    	if(status == false)
			    	{
			    		status = rshell(set);
			    	}
			    	
			    	set.clear();
			    }
	    	}
	    	else if(set.size() == 1 && set.at(0) == "#") break;
	    }
	    
	    //dealing with last base case, (where last string != connector)
	    if(set.size() != 0 && set.at(0) == ";")
		{
	    	popfront();
	    	
	    	rshell(set);
	    }	
	    else if(set.size() != 0 && set.at(0) == "&&")
	    {
		   	popfront();
		   	
	    	if(status == true)
	    	{
	    		rshell(set);
	    	}
	    }
	    else if(set.size() != 0 && set.at(0) == "||")
	    {
	    	popfront();
	    	
	    	if(status == false)
	    	{
	    		rshell(set);
	    	}
	    }
	    
    }

    return 0;
}

//function for calling children to action
bool rshell(vector<string>& set)
{
	while(!set.empty())
	{
		if(set.at(0) == "exit")
		{
			exit(0);
		}

    	
    	//Pid / Fork
    	pid_t pid = fork();
	    if(pid == -1)
	    {
	        perror("fork");
	    }
	    
	   
	    //Child
	    if(pid == 0)
	    {
		    
		    //Convert in char* in order to call execvp
		    char* cmd_exe[512];
		    //Load cmd_exe
		    for(int i = 0; i < static_cast<int>(set.size()); i++)
		    {
		    	cmd_exe[i] = (char*)set.at(i).c_str();
		    }
		    
		    //Set NULL ending the command
		    cmd_exe[set.size()] = NULL;
		    
		    //call execvp
		    if(execvp (cmd_exe[0], cmd_exe) == -1)
			{
		    	perror("exec");
		    	
		    	exit(-1);
		    }
		    
	    }
	   
	   
	    //Parent
	    if(pid > 0)
		{
			int status;
			
			//Wait
			waitpid(0, &status, 0);
			
			//Error
			if(status > 0)
			{
				return false;
			}
			
			return true;
		}
			
		set.clear();
	}
	return true;
}