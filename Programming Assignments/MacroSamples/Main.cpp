#include <iostream>
#include "Macro.h"
using namespace std;

#define FUNC_MACRO(T) DOUBLE_MACRO(T)*3
#define FUNC_MACRO1(T) T*3

int main(){
	int i = FUNC_MACRO(3);
	cout<<"Value is : "<<i<<endl;
	i = FUNC_MACRO1(3);
	cout<<"Value is : "<<i<<endl;

	return 0;
}
