#include "ClassDef.h"

void TestFunctionOne() {
	cout<<"Constructing the Object of Derived now..."<<endl<<endl;
	Derived object;

	cout<<endl<<"Destructing the Object of Derived now..."<<endl<<endl;
}

void TestFunctionTwo() {
	cout<<"Constructing the Object of DerivedAB now..."<<endl<<endl;
	DerivedAB object;

	cout<<endl<<"Destructing the Object of DerivedAB now..."<<endl<<endl;
}

void ClassSizeTest() {

	cout<<endl<<"Size of Base is : "<<sizeof(Base)<<endl;
	cout<<"Size of DerivedA is : "<<sizeof(DerivedA)<<endl;
	cout<<"Size of DerivedB is : "<<sizeof(DerivedB)<<endl;
	cout<<"Size of DerivedAB is : "<<sizeof(DerivedAB)<<endl;
	cout<<"Size of Derived is : "<<sizeof(Derived)<<endl;

}

int main() {
	TestFunctionOne();
	TestFunctionTwo();
	ClassSizeTest();
	
	return 0;
}