#include <stdio.h>

# include "Header.h"
# include "CHeader.h"

extern int globVar;
extern void foo();

void MyCFun() {
	printf("Hi");
	}

int main () {
	int* p = new int;
	void* p1 = p;
	delete p1;
	p1 = 0;
	delete p1;

	statVar = 40;
	globVar = 50;
	Sample s;
	
	cout<<"statVar : "<<statVar<<endl;
	cout<<"globVar : "<<globVar<<endl;
	cout<<"Sample::iStatVal : "<<s.iStatVal<<endl;

	function ();

	cout<<"statVar : "<<statVar<<endl;
	cout<<"globVar : "<<globVar<<endl;
	cout<<"Sample::iStatVal : "<<s.iStatVal<<endl;

	foo();
	//static_foo(); //Its Error, I cant call this fun as its declared as static in File1.c

	return 0;
}