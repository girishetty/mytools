#include <iostream>
using namespace std;

/*
int foo() {
	try{
		int m;
	}
	catch(...) {
		cout<<"hi";
	}
	catch(int i) {
		return 1;
}

/*
namespace N1 {
int f(int a) {
	return a*2;
}
}

namespace N2 {
int f(double a) {
	return a*3;
}
}
/*
class A{
	int aa;
public:
	A(int a) : aa(a) {}
};
*/

class A {
public:
	virtual void f() {cout<<"hello"<<endl;};
};

class B : public A {
public:
	void f() { }
	static void f(int i=0) {cout<<"hi"<<endl; }
};
int main() {
	//A c;
	//A a(10);
	//A b = a;
	B b;
	b.A::f();

	/*using N1::f;
	cout<<f(1.0)<<endl;

	using N2::f;
	cout<<f(1.0)<<endl; */
	return 0;
}

