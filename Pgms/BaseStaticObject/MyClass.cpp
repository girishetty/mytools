#include <iostream>
#include "MyClass.h"

using namespace std;

int* Base::p = NULL;

void Base::Display() {
	cout<<"* p is : "<<*p<<endl;
}
