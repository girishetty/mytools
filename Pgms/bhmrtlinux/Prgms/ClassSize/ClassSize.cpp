#include <iostream>
using namespace std;

class A {
	int a;
	char c;
};

class B : public A {
	char d;
	int i;
	static double stat;
};

class C {
	int int1;
	int int2;
	int i;
	long l;
	short s;
	char c;
};

class ClassA {
};
 
class ClassB{
};
 
class ClassC: public ClassA, public ClassB {
private:
	int a;
};
 

void ClassSizeTest () {
	cout<<"Testing Class Size"<<endl;
	cout<<"Size of A is : "<<sizeof(A)<<endl;
	cout<<"Size of B is : "<<sizeof(B)<<endl;
	cout<<"Size of C is : "<<sizeof(C)<<endl;
	cout<<"Size of ClassA is : "<<sizeof(ClassA)<<endl;
	cout<<"Size of ClassB is : "<<sizeof(ClassB)<<endl;
	cout<<"Size of ClassC is : "<<sizeof(ClassC)<<endl;
}

class ABase{
	int iMem;
};


class BBase : public virtual ABase {
	int iMem;
};

class CBase : public virtual ABase {
	int iMem;
};

class ABCDerived : public BBase, public CBase {
	int iMem;

public:
	void Foo() {
		cout<<"void ABCDerived::foo()"<<endl;
	}

	void Foo() const {
		cout<<"void ABCDerived::foo() const"<<endl;
	}
};

void TestVirtualInheritance() {
	cout<<"Testing Virtual Inheritance"<<endl;
	cout<<"Size of ABase : "<<sizeof(ABase)<<endl;
	cout<<"Size of BBase : "<<sizeof(BBase)<<endl;
	cout<<"Size of CBase : "<<sizeof(CBase)<<endl;
	cout<<"Size of ABCDerived : "<<sizeof(ABCDerived)<<endl;
}

void ConstFunCalling() {
	ABCDerived obj1;
	obj1.Foo();
	const ABCDerived obj2;
	obj2.Foo();
}

void foo() {
	cout<<"void foo()"<<endl;
}
/*
void foo() const {
	cout<<"void foo() const"<<endl;
}
*/
int main() {
	ClassSizeTest();
	TestVirtualInheritance();
	ConstFunCalling();
	foo();
	return 0;
}

