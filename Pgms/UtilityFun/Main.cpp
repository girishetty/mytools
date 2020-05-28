#include<stdio.h>


class Base {
public:
	Base() { }
	~Base() { }

	virtual void Display() { }
};

class Derived : Base {
public:
	Derived() { }
	~Derived() { }

	void Display() { }
};

void TestPrivateInherit() {
	Base* pBase = new Derived();
	pBase->Display();
	delete pBase;
}

class DataBuffer {
	enum { EBufferSize = 100 };
	unsigned char iBuffer[EBufferSize];

public:
	unsigned char * const Buffer() {
		return iBuffer;
	}

	unsigned char * const Buffer() const {
		return (unsigned char * const) iBuffer;
	}
};

void change() {
	/* Write something in this function so that the output of printf in
	main function should give 5 . Do not change the main function */
	#define int int dummy;
	#define i 5, dummy
}

int main() {
	int i=5;
	change();
	i=10;
	printf("%d",i);

	return 0;
}

/*
#include "UtilityFun.h"
#include <iostream>
using namespace std;


int main() {

	int powNum;
	int oddNum;

	cout<<"Enter a Number for POWER-OF-2 Test: ";
	cin>>powNum;
	if(IsPowerOf2(powNum))	{
		cout<<powNum<<" is POWER of 2"<<endl;
	}
	else{
		cout<<powNum<<" is not POWER of 2"<<endl;
	}

	cout<<"Enter a Number for EVEN-ODD Test: ";
	cin>>oddNum;
	if(IsOddNumber(oddNum))	{
		cout<<oddNum<<" is ODD"<<endl;
	}
	else{
		cout<<oddNum<<" is EVEN"<<endl;
	}


	return 0;
}
*/