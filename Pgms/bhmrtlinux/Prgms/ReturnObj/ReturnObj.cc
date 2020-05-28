#include <iostream>
using namespace std;

class A {
	int a;
	int* p;
public:
	A() : a(10), p(0) {
		cout<<"In A::A()"<<endl;
	}

	A(const A& obj): a(obj.a), p(0) {
		cout<<"In A::A(const A&)"<<endl;
	}

	A operator= ( const A& obj ) {
		a = obj.a;
		cout<<"In A::Assignment Optr"<<endl;
	}

	~A() {
		cout<<"In A::~A()"<<endl;
		delete p;
	}

	A Foo( ) {
		cout<<"In A::Foo "<<a<<endl;
		}
	};

A LocalFun() {
	cout<<"In LocalFun"<<endl;
	}

int main() {
	A obj1;
	A obj2;
	obj1 = obj2;
	//A a = Foo();
	//cout<<"After Foo"<<endl;
	//a.Foo();

	//cout<<"After A::Foo"<<endl;
}

