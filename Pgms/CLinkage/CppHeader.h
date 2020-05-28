#ifndef CPPHEADER_H
#define CPPHEADER_H

#include <iostream>
using namespace std;

void Foo() {
	cout<<"Foo"<<endl;
}

void Foo(int a) {
	cout<<"Foo "<<"a is "<<a<<endl;
}

extern "C" {

class Sample{
public:
	Sample(int a = 10) : iMem(a) { }
	void Display() { cout<<"iMem is : "<<iMem<<endl; }
	void Display(int a) { cout<<"iMem is : "<<iMem<<" a is : "<<a<<endl; }

private:
	int iMem;
};

/*
template<class DataType> void Foo(DataType data) {
	}*/
}

#endif// CPPHEADER_H