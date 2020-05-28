#ifndef HEADER_H
#define HEADER_H

#include <iostream>
using namespace std;

void ShowClassLayout(char* aClassName, int* aPtr, int aLength){
	cout<<"Object Layout of "<<aClassName<<" is......"<<endl;
	for(;aLength;) {
		cout<<"Address : "<<aPtr<<" --> Content : "<<*aPtr<<endl;
		aPtr++;
		aLength -= 4;
	}
}

void CallVirtualFunction(char* aClassName, long* aVPTR, int aCount) {
	cout<<"Calling Virtual Functions on : "<<aClassName<<" Pointer"<<endl;
	long* pVTABLE = (long*) *aVPTR;

	typedef void (*FUN_PTR)(void);
	FUN_PTR fPTR;
	
	for(int i=0; i<aCount; i++) {
		fPTR = (FUN_PTR)pVTABLE[i];
		fPTR();
	}
}


#endif //HEADER_H
