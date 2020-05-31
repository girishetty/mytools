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

void TestFunctionThree(){
	cout<<"Constructing the Object of Derived now..."<<endl<<endl;
	Base* p = new Derived;

	cout<<endl<<"Destructing the Object of Derived now..."<<endl<<endl;
	delete p;
}

void ClassSizeTest() {
	Derived* pD = new Derived;
	DerivedAB* pAB = pD;
	DerivedB* pDB = pD;
	DerivedA* pDA = pD;
	Base* pB = pD;
	BaseMost* pBM = pD;

	cout<<endl<<"Pointer to Derived using pBM is "<<pBM<<endl;
	cout<<"Pointer to Derived using pB is "<<pB<<endl;
	cout<<"Pointer to Derived using pDA is "<<pDA<<endl;
	cout<<"Pointer to Derived using pDB is "<<pDB<<endl;
	cout<<"Pointer to Derived using pAB is "<<pAB<<endl;
	cout<<"Pointer to Derived using pD is "<<pD<<endl<<endl;

	pBM->MemberAddressBM();
	pB->MemberAddressB();
	pDA->MemberAddressDA();
	pDB->MemberAddressDB();
	pAB->MemberAddressDAB();
	pD->MemberAddressD();

	cout<<endl<<"Size of BaseMost is : "<<sizeof(BaseMost)<<endl;
	cout<<"Size of Base is : "<<sizeof(Base)<<endl;
	cout<<"Size of DerivedA is : "<<sizeof(DerivedA)<<endl;
	cout<<"Size of DerivedB is : "<<sizeof(DerivedB)<<endl;
	cout<<"Size of DerivedAB is : "<<sizeof(DerivedAB)<<endl;
	cout<<"Size of Derived is : "<<sizeof(Derived)<<endl;
}

void TestSisterDelegation(){
	Join* p1 = new Join;
	Derived1* p2 = p1;
	Derived2* p3 = p1;

	cout<<"Call Display using Join Class"<<endl;
	p1->DisplayA();
	p1->DisplayB();

	cout<<endl<<"Call Display using Derived1 Class"<<endl;
	p2->DisplayA();
	p2->DisplayB();

	cout<<endl<<"Call Display using Derived2 Class"<<endl;
	p3->DisplayA();
	p3->DisplayB();

	//Here all delete will do the same operation
	//delete p1;
	//delete p2;
	delete p3;
	//Here we can not instantiate either of Derived1 or Derived2
	//p2 = new Derived1;
	//p3 = new Derived3;
}

void ClassSizeSampleTest() {
	cout<<"Size of A is : "<<sizeof(A)<<endl;
	cout<<"Size of B is : "<<sizeof(B)<<endl;
	cout<<"Size of C is : "<<sizeof(C)<<endl;
	cout<<"Size of D is : "<<sizeof(D)<<endl;
}

int main() {
	//TestFunctionOne();
	//TestFunctionTwo();
	//TestFunctionThree();
	//ClassSizeTest();
	ClassSizeSampleTest();
	//TestSisterDelegation();
	
	return 0;
}
