#include <iostream>
using namespace std;

class Base {
	int a;
public:
        virtual void Display() {cout<<"Base"<<endl;}
};

class Derived : public Base {
	int b;
public:
        virtual void Display() {cout<<"Derived "<<endl;}
};

#if 0
void Function(Base*& ptr) {
        ptr = new Derived;
}

void Function(Base** ptr) {
        *ptr = new Derived;
}
#endif

void Function(void* ptr) {
        Derived** p = (Derived**)ptr;
        *p = new Derived;
}

void DisplaySize() {
	Base* p = NULL;
	Base** p1 = &p;

	cout<<"p is: "<<p<<endl;
	cout<<"++p is: "<<++p<<endl;
	cout<<"p1 is: "<<p1<<endl;
	cout<<"++p1 is: "<<++p1<<endl;

	Derived* q = NULL;
	Derived** q1 = &q;

	cout<<"q is: "<<q<<endl;
	cout<<"++q is: "<<++q<<endl;
	cout<<"q1 is: "<<q1<<endl;
	cout<<"++q1 is: "<<++q1<<endl;
}

int main() {
        Derived* ptr = NULL;
        Function(&ptr);
        cout<<ptr<<endl;
        ptr->Display();
        delete ptr;

		DisplaySize();
        return 0;
}
