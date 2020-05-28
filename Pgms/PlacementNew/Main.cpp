#include "Header.h"
using namespace std;
#include <stdio.h>

int MyClass::iObjCount = 0;

static inline void* operator new(size_t aSize) {
	MyClass::iObjCount++;
	return malloc(aSize);
}

static inline void operator delete(void* aPtr) {
	free(aPtr);
}

void OtherFun();

int main() {
	MyClass* p = new MyClass;
	delete p;

	OtherFun();
	cout<<"Valuse of MyClass::iObjCount is : "<<MyClass::iObjCount<<endl;
	return 0;
}
