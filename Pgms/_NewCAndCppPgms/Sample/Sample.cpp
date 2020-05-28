#include <iostream>
using namespace std;

class Sample {
	int iVal;

public:
	explicit Sample(int aParam = 0) : iVal(aParam) {
		cout<<"Sample constructed with "<<iVal<<endl;
	}

	~Sample() {
		cout<<"Sample destroyed..."<<endl;
	}
};

int main() {
	Sample s1;
	Sample s2(); //This wont invoke constructer, this is function dclaration
	Sample s3(2);
	Sample s4(0);
	//Sample s5 = 10; //This is error as constructer is declared as explicit

	return 0;
}

