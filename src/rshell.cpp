#include <iostream>
#include <vector>

//Enable getlogin function
#include <unistd.h>

using namespace std;

//Base Class: Line
class Line
{
	private:
		vector<Line> data;
	public:
		void listener();
		virtual void process();
};

//Inheritances: Solo, Duo, Connector
class Solo: public Line
{
	public:
		void process();
};

void initializeUser()
{
	char* user = getlogin();
	//char* host = gethostname();
	//Guards to be added for NULL cases
	//Host to be added
	if(user == NULL)
	{
		cout << "Error: Unidentified User." << endl;
	}
	else
	{
		cout << user << '@' << "$ ";
	}
}

int main()
{
	//Assumption
	string c;

    //cout << "Hello World!!" << endl;
	while(c != "exit")
	{
		initializeUser();
		cin >> c;
	}
    return 0;
}
