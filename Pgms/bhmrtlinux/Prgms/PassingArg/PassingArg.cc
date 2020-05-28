#include <iostream>
using namespace std;

class Base {

public:
	virtual void Display() {cout<<"Base"<<endl;}
};

class Derived : public Base {

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

int main() {
	Derived* ptr = NULL;
	Function(&ptr);
	cout<<ptr<<endl;
	ptr->Display();
	delete ptr;

	return 0;
}

