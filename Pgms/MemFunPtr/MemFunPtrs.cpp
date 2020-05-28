#include <iostream>
using namespace std;


class MyClass;

typedef int (MyClass::*FuncPtr) ();
typedef int (*GenFuncPtr) ();
FuncPtr two = NULL;
GenFuncPtr ptr = NULL;

class MyClass {
public:
	int func_one();
private:
	int func_two();
};


int MyClass::func_one() {
	cout<<"MyClass::func_one()"<<endl;
	two = &MyClass::func_two;
	//ptr = reinterpret_cast<GenFuncPtr> (&MyClass::func_two); // how do we cast a normal function pointer as class member funciton ptr?
	return 1;
}

int MyClass::func_two() {
	cout<<"MyClass::func_two()"<<endl;
	return 1;
}

int ordinary_function() {
	cout<<"ordinary_function()"<<endl;
	return 0;
}

int main()
{
	MyClass instance;
	FuncPtr one = &MyClass::func_one;
	//FuncPtr three = &MyClass::func_two;   //Compiler error, can not access private member of the class
	(instance.*one)();
	(instance.*two)();
	ptr = ordinary_function;
	ptr();
	return 0;
}
