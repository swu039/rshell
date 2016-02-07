#include <iostream>
#include <vector>

//Enable getlogin function
#include <unistd.h>
//Enable variable size of buffer
#include <stdio.h>
using namespace std;

//Global Variables
const unsigned buffSize = BUFSIZ;

//Base Class: Line
class Line
{
	private:
		vector<Line> data;
	public:
		//void listener();
		virtual void process() = 0;
};

//Listener - Takes user input
string listener()
{
	string command;

	getline(cin, command);
	//cout << "Your commands: " <<  command << endl;
	
	//Parse
	return command;
}

//Inheritances: Solo, Duo, Connector
class Solo: public Line
{
	public:
		void process();
};

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
		cout << "Error: Unidentified User." << endl;
	}
	else if(hostID == -1)
	{
		cout << "Error: Unidentified Host." << endl;
	}
	else
	{
		cout << user << '@' << host << "$ ";
	}
}

int main()
{
	//Assumption
	string c;

    //cout << "Hello World!!" << endl;
    //Our structure should work: while(Base->process() != "exit")
    //Or something....
	while(c != "exit")
	{
		initializeUser();
		c = listener();
	}
    return 0;
}
