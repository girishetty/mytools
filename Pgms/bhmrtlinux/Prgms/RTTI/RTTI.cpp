#include <iostream>
#include <typeinfo>


using namespace std;

class Base {
	int iMem;
public:
	Base(int aMem = 0) : iMem(aMem){ };
	virtual ~Base() { };
};

class Derived1 : public Base {
};

class Derived2 : public Base {
};

class Derived3 : public Base {
};

void Foo(Base* aBase) {
        cout<<"aBase Points to Object of Type : "<<typeid(*aBase).name()<<endl;
        Derived1* d1 = dynamic_cast<Derived1*> (aBase);
        if(d1) {
                cout<<"Base is pointing to Object of Type Derived1"<<endl;
	}
	else{
                Derived2* d2 = dynamic_cast<Derived2*>(aBase);
                if(d2) {
                        cout<<"Base is pointing to Object of Type Derived2"<<endl;
		}
		else {
                        Derived3* d3 = dynamic_cast<Derived3*>(aBase);
                        if(d3) {
                                cout<<"Base is pointing to Object of Type Derived3"<<endl;
				}
			else{
				cout<<"Impossible !!!!"<<endl;
			}
                }
        }
}

int main() {
	Derived1 d1;
	Derived2 d2;
	Derived3 d3;
	Foo(&d1);
	Foo(&d2);
	Foo(&d3);

	return 0;
}


