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

int main()
{
	//rshell

    //fork
    while(1)
    {
    	//Pipe array: index 0 = read, index 1 = write
		int fd[2];
		char atad[10];
    	
    	
    	//Pipe
		if(pipe(fd) == -1)
		{
			perror("pipe");
		}
		
		string command;
    	pid_t pid = fork();
		//pid
	    if(pid == -1)
	    {
	        perror("fork");
	    }
	    
	    //Child
	    if(pid == 0)
	    {
			cout << "---Child---" << endl;
	    	
	    	//rshell prompt
	    	initializeUser();
			//bool flag;
			
			//retrieve line
			getline(cin, command);
		
			char instruct[command.size() + 1];
			
		    // Load command into instruct
		    for(int i = 0; i < command.size(); i++)
		    {
		        instruct[i] = command.at(i);
		    }
		    
			//NULL
			instruct[command.size()] = '\0';
			//first Initalization
			vector<string> cmds;
			char* token = strtok((char*)instruct, " ");
			
			
			//PIPING
			string s = token;
			if(s == "exit")
	        {
	        	//cout << "W" << endl;
	    		write(fd[1],"yes\0", 4);
	    		exit(1);
	        }
	        else
	        {
	        	
	    		write(fd[1],"no\0", 4);
	        }
			//PIPING
			
			
			
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
		        if(*token == ';')
		        {
		        	break;
		        }
		        cmds.push_back(token);
		    }
		    
		    //Convert in char* in order to call execvp
		    char* cmd_exe[cmds.size() + 1];
		    
		    //Load cmd_exe
		    for(int i = 0; i < cmds.size(); i++)
		    {
		    	cmd_exe[i] = (char*)cmds.at(i).c_str();
		    	//cout << "cmd " << i << " is: " << cmd_exe[i] << endl;
		    }
		    
		    //Set NULL ending the command
		    cmd_exe[cmds.size()] = NULL;
		    //cout << "cmd " << cmds.size() << " is: " << cmd_exe[cmds.size()] << endl;
		    
		    //call execvp
		    if(execvp (cmd_exe[0], cmd_exe) == -1)
			{
		    	perror("exec");
		    }
	    }
	    
	    //Parent
	    if(pid > 0)
		{
			//Close writing
			//close(fd[1]);
			if(wait(0) == -1)
			{
				perror("wait");
			}
			
			cout << "---Parent---" << endl;
			read(fd[0], atad, 4);
	
			//cout << "atad: " << atad << endl;
				
			string s = atad;
			//cout << "s: " << s << endl;
			if(s == "yes")
			{
				cout << "Now Killing Parent" << endl;
				exit(1);
			}
		}
    }
    
	
	
	
	
    return 0;
}
