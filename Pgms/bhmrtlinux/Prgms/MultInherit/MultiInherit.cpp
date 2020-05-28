# include <iostream>
using namespace std;

class A {
public:
	virtual ~A() { }
};

class B {
public:
	virtual ~B() { }
};


class C {
public:
	//virtual ~C() { }
};

class D : public virtual A, public virtual B, public virtual C {
//class D : public A, public B, public C {
public:
	//virtual ~D() { }
};

void ClassSizeSampleTest() {
	cout<<"Size of A is : "<<sizeof(A)<<endl;
	cout<<"Size of B is : "<<sizeof(B)<<endl;
	cout<<"Size of C is : "<<sizeof(C)<<endl;
	cout<<"Size of D is : "<<sizeof(D)<<endl;
}

int main() {
	ClassSizeSampleTest();
	return 0;
}

