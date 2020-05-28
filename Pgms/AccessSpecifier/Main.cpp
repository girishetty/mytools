#include <iostream>
using namespace std;

class Base{
public:
	Base(int aVal = 10 ) : iMem(aVal) {
		cout<<"Base::Base()"<<endl;
	}

	void virtual VirtualFun() {
		cout<<"Base::VirtualFun() : iMem is "<<iMem<<endl;
	}

private:
	int iMem;
};

class Derived : public Base {
public:
	Derived(int aVal = 10 ) : Base(aVal), iMem(aVal+1) {
		cout<<"Derived::Derived()"<<endl;
	}

	void virtual VirtualFun() {
		cout<<"Derived::VirtualFun() : iMem is "<<iMem<<endl;
	}

private:
	int iMem;
};


int main() {
	Base* pBase = new Derived(2);
	pBase->VirtualFun();

	delete pBase;

	return 0;
}
