#include <iostream>
#include <vector>

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

int main()
{
    cout << "Hello World!!" << endl;
    return 0;
}
