# include "Header.h"

int globVar = 30;

int Sample::iStatVal = 30;

void foo() {
	cout<<"Foo"<<endl;
}

static void static_foo() {
	cout<<"static_foo"<<endl;
}

void function () {
	statVar = 100;
	globVar = 200;

	cout<<"Sample::iStatVal : "<<Sample::iStatVal<<endl;
	
	Sample s;
	s.iStatVal = 400;

	cout<<"statVar : "<<statVar<<endl;
	cout<<"globVar : "<<globVar<<endl;
	cout<<"Sample::iStatVal : "<<s.iStatVal<<endl;
}