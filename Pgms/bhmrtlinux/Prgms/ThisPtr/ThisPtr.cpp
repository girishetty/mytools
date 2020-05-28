/*
This example code Illustrates:
(i)     Dereferencing a null pointer value to invoke a member function
results in undefined behaviour
(ii)    how this pointer is been uded inside a class.


If the function is a nonstatic member function, the .this. parameter of the function shall be initialized with a pointer to the object of the call

There exists the implied assertion that, "inside a non-static member function this pointer wont be 0 (this != 0)".
*/
# include <iostream>
using namespace std;

class ClassA {
public:
	void MethodA() {
		cout<<"ClassA::MethodA()   : this is : "<<this<<endl;
	}
private:
	int iMemA;
};


class ClassB {
public:
	void MethodB(){
		cout<<"ClassB::MethodB()   : this is : "<<this<<endl;
	}
private:
	int iMemB;
};


class ClassAB: public ClassA, public ClassB {
public:

/*
While calling MethodB, which is defined in ClassB, the compiler adjustes
the "this" pointer to the location where it would normally expect the
ClassB sub-object.  That is 4 bytes offset (size of ClassA).

So, while calling MethodA(), it sends same "this" ptr (same as "this" of
ClassAB), but while calling MethodB, it adds 4 to "this" pointer and sends.

You can check this one in the output
*/
	void MethodAB() {
		cout<<"ClassAB::MethodAB() : this is : "<<this<<endl;
		MethodA();
		MethodB();
	}
private:
	int iMemAB;
};

/*
Clearly, the compiler adjusted the "this" pointer to the location where it
would normally expect the ClassB sub-object. That is 4 bytes offset (size of ClassA)
*/
void TestWithPtr () {
	ClassAB* pAB = 0;
	ClassA* pA = 0;
	ClassB* pB = 0;

	cout<<endl<<"Calling using Pointer(NULL) to ClassAB"<<endl;
	pAB->MethodAB();
	
	cout<<endl<<"Calling using Pointer(NULL) to ClassA"<<endl;
	pA->MethodA();

	cout<<endl<<"Calling using Pointer(NULL) to ClassB"<<endl;
	pB->MethodB();
}

void TestWithStackVar () {
	ClassAB ab;
	ClassA a;
	ClassB b;

	cout<<endl<<"Calling using Object of ClassAB"<<endl;
	ab.MethodAB();
	
	cout<<endl<<"Calling using Object of ClassA"<<endl;
	a.MethodA();
	
	cout<<endl<<"Calling using Object of ClassB"<<endl;
	b.MethodB();
}

int main() {
	TestWithStackVar ();
	TestWithPtr ();

	return 0;
}

