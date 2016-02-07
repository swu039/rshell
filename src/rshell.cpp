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
	if(user == NULL)
	{
		cout << "Error: Unidentified User." << endl;
	}
	else
	{
		cout << user << '@' << '$' << endl;
	}
}

int main()
{
    cout << "Hello World!!" << endl;
	initializeUser();
    return 0;
}
