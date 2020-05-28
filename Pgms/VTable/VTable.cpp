#include<iostream>
using namespace std;


class CA
{
public:
	virtual void fun()
	{
		cout<<"CA fun"<<endl;		
	}
	virtual void fun2()
	{
		cout<<"CA fun2"<<endl;		
	}
};

void main()
{
	CA *pa = new CA();
	
	long* vptr = (long*) pa;
	
	long* vtable = (long*) *vptr;

	typedef void (*FUNPTR)();
	
	FUNPTR pfun = (FUNPTR) vtable[1];
	
	pfun();
	pfun = (FUNPTR) vtable[0];
	pfun();
	delete pa;
}
