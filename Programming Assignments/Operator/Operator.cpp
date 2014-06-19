#include <iostream>
using namespace std;

class Base {
private:
	int array [10];
	int size;

public:
	Base (int s = 10) {
		if(s > 10)
			s = 10;
		size = s;
		
		for(int i=0;i<size;i++)
			array[i] = i+1;
	}
	Base (Base& base) {
		size = base.size;
		for(int i=0;i<size;i++)
			array[i] = base.array[i];
	}

	int& operator[](int i) {
		return array[i];
	}
};

class Derived : public Base {
public :
	Derived (int s=10) : Base (s) {
	}
	Derived (Derived& d) : Base ((Base&) d) {
	}
};

//Access Control...
class A {
public:
	int f ();
	int g ;
private:
	int p ;
};

class B : private A {
public:
	A::f;  // Make public
	A::p;  // Make public
protected:
	A::g ; // Make protected

};


int main () {
	Derived d;
	Derived d1=d;

	cout<<d[4]<<endl;
	cout<<d1[5]<<endl;
	return 0;
}