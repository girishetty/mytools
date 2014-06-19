#include <iostream>
using namespace std;


class A {
public:
	A() {
		cout << "A::A()" << endl;
    }
	
	~A() {
		cout << "A::~A()" << endl;
		throw "A::exception";
	}
};

class B {
public:
	B() {
		cout << "B::B()" << endl;
		throw "B::exception";
	}
	
	~B() {
		cout << "B::~B()";
	}
};

void ExceptionTest () {
	try {
		cout << "Entering try...catch block" << endl;
		A	objectA;
		B	objectB;
		cout << "Exiting try...catch block" << endl;
	}catch (char* ex) {
		cout << ex << endl;
	}
}

	
	int main(int, char**)  {
	try {
		ExceptionTest ();
	}catch (char* ex) {
		cout << ex << endl;
	}

	return 0;
}
