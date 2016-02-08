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



//Listener - Takes user input
string listener()
{
	string command;

	getline(cin, command);
	//cout << "Your commands: " <<  command << endl;
	
	//Parse
	return command;
}

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
	    	//rshell prompt
	    	initializeUser();
		
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

			//Push main command
			cmds.push_back(token);
			
			/*
			Exit Piece
			if(init == "exit")
			{
				//Adding piece
				char* b[3];
				
				string Rune = "touch";
				string Soul = "killme.txt";
				
				b[0] = (char*)Rune.c_str();
				
				b[1] = (char*)Soul.c_str();
		    	b[2] = NULL;
				
				
				cout << "Time to die!" << endl;
				if(execvp (b[0], b) == -1)
				{
					//cout << "Called execvp" << endl;
			    	perror("exec");
			    }
				
			}
			
			*/
		    
		    //Insert arguments
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
			if(wait(0) == -1)
			{
				perror("wait");
			}
			
			//Removing piece
			// char* b[3];
			
			// string Rune = "rm";
			// string Soul = "killme.txt";
			
			// b[0] = (char*)Rune.c_str();
			
			// b[1] = (char*)Soul.c_str();
	  //  	b[2] = NULL;

			// if(execvp (b[0], b) == -1)
			// {
				
			// 	//cout << "Called execvp" << endl;
		 //   	perror("exec");
		 //   }
				
		}
		
    }
    
	
	
	
	
    return 0;
}
