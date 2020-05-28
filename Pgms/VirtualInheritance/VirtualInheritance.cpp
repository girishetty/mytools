#include <iostream.h>
#include"VirtualInheritance.h"

void VirtualInheritanceClassSize () {
	cout<<"Size of Base : "<<sizeof(Base)<<endl;
	cout<<"Size of DereivedA : "<<sizeof(DereivedA)<<endl;
	cout<<"Size of DereivedB : "<<sizeof(DereivedB)<<endl;
	cout<<"Size of DereivedC : "<<sizeof(DereivedC)<<endl;
	cout<<"Size of Dereived : "<<sizeof(Dereived)<<endl;
}

void FinalClassTest () {
	DD d;
}

void testVirtualInheritance() {
	ABCDerived d(7);
}

int main () {
	VirtualInheritanceClassSize();
	FinalClassTest();
	testVirtualInheritance();

	return 0;
}
