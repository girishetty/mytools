#include<string.h>
#include <iostream>
using namespace std;

#if 1

enum Enu{
	EVal1,
	EVal2,
	EVal3 = 10,
	EVal4
};

class Base{
public:
                  virtual void func(void){
                                cout<<"In base func";
                  }
};

class Derived: public Base{
public:
                    void func(void){
                        //cout<<"In Derived func"<<endl;
                        Base:func();
                        cout<<"In Derived func end"<<endl;
                    }
};

int foo(){
        Base* b = new Derived();
        b->func();
        return 0;
}

class A{
public:
    virtual ~A(){ cout<<"A destr"<<endl; };
    A(){ cout<<"A constr"<<endl;  };
};

class B : public A{
public:
    ~B(){ cout<<"B destr"<<endl; };
    B(){ cout<<"B constr"<<endl;  };
};

class C : public B{
public:
    ~C(){ cout<<"C destr"<<endl; };
    C(){ cout<<"C constr"<<endl;  };
};

int main(){
#if 1
    //foo();
    C* p = new C();
	cout<<endl<<endl;
    delete p;
//#else
    cout<<"Val1: "<<EVal1<<endl;
	cout<<"Val2: "<<EVal2<<endl;
	cout<<"Val3: "<<EVal3<<endl;
	cout<<"Val4: "<<EVal4<<endl;
#endif
    return 0;
}

#else
typedef unsigned int BOOL;
#define TRUE 1
#define FALSE 0

int main() {

	BOOL bArrayOne[10] = { TRUE };
	BOOL bArrayTwo[1000] = { FALSE };
	BOOL bArrayThree[10];

	int* p = NULL;
	short* q = NULL;
	char* r = (char*) (p + 1);
	char* s = (char*) (q + 1);

	int signed_int = -2;
	unsigned int unsigned_int = 60;
	cout<<"signed_int : "<<signed_int<<endl;
	cout<<"unsigned_int : "<<unsigned_int<<endl;
	
	unsigned_int += signed_int;
	cout<<"unsigned_int : "<<unsigned_int<<endl;

	unsigned_int = 0xFFFF0000;
	signed_int -= unsigned_int;
	cout<<"signed_int : "<<signed_int<<endl;
	cout<<"unsigned_int : "<<unsigned_int<<endl;


	cout<<"p : "<<p<<endl;
	cout<<"q : "<<q<<endl;
	cout<<"r : "<<(int*)r<<endl;
	cout<<"s : "<<(short*)s<<endl;

	memset(bArrayTwo, FALSE, sizeof(BOOL) * 10);

	for(int i=0; i<10; i++) 
		cout<<bArrayOne[i]<<" ";

	cout<<endl;
	for(int i=0; i<10; i++) 
		cout<<bArrayTwo[i]<<" ";

	return 0;
}
#endif
