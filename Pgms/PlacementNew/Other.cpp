#include "Header.h"
using namespace std;
#include <stdio.h>

void OtherFun() {
	MyClass* p = ::new MyClass;
	delete p;
}
