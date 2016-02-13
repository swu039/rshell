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

		cout << "fakeprompt@weoweo$ ";
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
	    bool status = true;
	    vector<string> set;
	    int j = 0;
	    
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


bool rshell(vector<string>& set)
{
	while(!set.empty())
	{
		
		//Pipe array: index 0 = read, index 1 = write
		// int fd[2];
		// char atad[10];
		
		
		//cout << cmds.at(0) << endl;
		if(set.at(0) == "exit")
		{
			//cout << "BREAK" << endl;
			exit(0);
		}
		
		//Pipe
		// if(pipe(fd) == -1)
		// {
		// 	perror("pipe");
		// }

    	
    	//Pid / Fork
    	pid_t pid = fork();
	    if(pid == -1)
	    {
	        perror("fork");
	    }
	    
	   
	    //Child
	    if(pid == 0)
	    {
			//cout << "---Child---" << endl;
		    
		    //Convert in char* in order to call execvp
		    char* cmd_exe[512];
		    //Load cmd_exe
		    for(int i = 0; i < static_cast<int>(set.size()); i++)
		    {
		    	cmd_exe[i] = (char*)set.at(i).c_str();
		    	//cout << "cmd " << i << " is: " << cmd_exe[i] << endl;
		    }
		    
		    //Set NULL ending the command
		    cmd_exe[set.size()] = NULL;
		    //cmd_exe[set.size()] = '\0';
		    //cout << "cmd " << cmds.size() << " is: " << cmd_exe[cmds.size()] << endl;
		    
		    //write(fd[1],"pass\0", 5);
		    	
		    
		    //call execvp
		    if(execvp (cmd_exe[0], cmd_exe) == -1)
			{
		    	perror("exec");
		    	//DEBUG HERE
		    	//write(fd[1],"fail\0", 5);
		    	
		    	exit(-1);
		    }

			//write(fd[1],"pass\0", 5);
		    
	    }
	   
	   
	    //Parent
	    if(pid > 0)
		{
			// if(wait(0) == -1)
			// {
			// 	perror("wait");
			// }
			int status;
			
			//Wait
			waitpid(0, &status, 0);
			
			
			//cout << "Status: " << status << endl;
			//Error
			if(status > 0)
			{
				return false;
			}
			
			//cout << status << endl;
			//cout << "---Parent---" << endl;
			//Read from pipe
			//read(fd[0], atad, 5);
	
			//cout << "atad: " << atad << endl;
				
			//string s = atad;
			//cout << "s: " << s << endl;
			// if(s == "fail")
			// {
			// 	cout << "Now Killing Parent" << endl;
			// 	// Exit rshell
			// 	//exit(0);
			// 	return false;
			// }
			
			return true;
		}
			
		set.clear();
	}
	return true;
}