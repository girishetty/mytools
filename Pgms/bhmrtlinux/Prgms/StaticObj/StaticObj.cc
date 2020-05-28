#include <iostream>
using namespace std;


class Sample {
public:
	Sample() {
		cout<<"Constructing Sample"<<endl;
	}
	~Sample() {
		cout<<"Destructing Sample"<<endl;
	}
};

void CallMe(){
	cout<<"Entered CallMe"<<endl;
	static Sample sam;
	cout<<"Exiting CallMe"<<endl;
}

int main() {
	cout<<"Entered Main"<<endl;
	CallMe();
	cout<<"Exiting Main"<<endl;
	return 0;
}



