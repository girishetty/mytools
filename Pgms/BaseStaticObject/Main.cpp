#include <iostream>
#include <string>
#include "MyClass.h"

using namespace std;

class MyClass
{

private:
	//string& iVal;
	//int& iVal;
	const int iVal;

public:
	MyClass(int val = 10) : iVal(val) {}

	void Display() {cout<<"Val is : "<<iVal<<endl; }

	MyClass& operator=(MyClass& obj) {
		//iVal = obj.iVal;
		return *this;
	}

};

void foo() {
	MyClass aa1;
	MyClass aa2(20);

	aa1.Display();
	aa2.Display();

	MyClass& ra = aa1;
	ra.Display();
	MyClass& rb = aa2;
	rb.Display();

	ra = rb;
	ra.Display();
}


int main() {
#if 0
	DerivedA* p1 = new DerivedA;
	DerivedB* p2 = new DerivedB;
	DerivedC* p3 = new DerivedC;

	p1->Display();
	p3->Display();
	p2->Display();

	delete p1;
	delete p2;
	delete p3;
#endif

	foo();
	return 0;
}
