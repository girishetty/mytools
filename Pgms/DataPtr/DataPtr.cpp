#include <iostream>
using namespace std;

struct MyStruct {
	int   a;
	int   b;
	int*  p;
	void foo();
	MyStruct(int x=20, int y=10) : a(x), b(y) {
		p = new int(50);
	}
	~MyStruct(){
		delete p;
	}
};

int main() {
	int   MyStruct::*pA = &MyStruct::a;
	int   MyStruct::*pB = &MyStruct::b;
	int*  MyStruct::*pP = &MyStruct::p;
	MyStruct s;

	cout<<"Value of s.a is : "<<s.*pA<<endl;
	cout<<"Value of s.a is : "<<s.a<<endl;
	cout<<"Value of s.b is : "<<s.*pB<<endl;
	cout<<"Value of s.b is : "<<s.b<<endl;
	cout<<"Value of s.p is : "<<s.*pP<<endl;
	cout<<"Value of s.p is : "<<s.p<<endl;
	cout<<"Value of *s.p is : "<<*(s.*pP)<<endl;
	cout<<"Value of *s.p is : "<<*(s.p)<<endl;
	return 0;
}
