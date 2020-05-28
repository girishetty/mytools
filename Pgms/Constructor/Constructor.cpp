#include <iostream>
using namespace std;

class Sample {
	int iMem;

public:
	Sample(int aMem = 0 ) : iMem(aMem) {
		cout<<"Default/Int Constructor"<<endl;
	}

	Sample(Sample& aSam) : iMem(aSam.iMem) {
		cout<<"Copy Constructor"<<endl;
	}
};


void Foo() {
	static s(10);
}

int main() {
	cout<<"Entered Main"<<endl;
	Foo();
	/*
	Sample s1(10);
	Sample s2 = 10;
	Sample s3 = s1;
	*/
	cout<<"Exiting from Main"<<endl;
	return 0;
}
