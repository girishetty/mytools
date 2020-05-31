#ifndef VIRTUAL_H
#define VIRTUAL_H

#include "Header.h"

class Base{
	int iMem;

public:
	Base(int aMem=0) : iMem(aMem) {
		ShowClassLayout("Base", (int*)this, sizeof(*this));
	}

	virtual void Function1(){
		cout<<"Base::Function1()"<<endl;
	}

	virtual void Function2(){
		cout<<"Base::Function2()"<<endl;
	}

	virtual ~Base(){
		cout<<"Base destroyed..."<<endl;
	}
};

class DerivedA : virtual public Base {
	int iMem;

public:
	DerivedA(int aMem=0) : Base(aMem), iMem(aMem+1) {
		ShowClassLayout("DerivedA", (int*)this, sizeof(*this));
	}

	virtual void Function3(){
		cout<<"DerivedA::Function3()"<<endl;
	}

	virtual void Function4(){
		cout<<"DerivedA::Function4()"<<endl;
	}

	virtual ~DerivedA(){
		cout<<"DerivedA destroyed..."<<endl;
	}

};

class DerivedB : virtual public Base {
	int iMem;

public:
	DerivedB(int aMem=0) : Base(aMem), iMem(aMem+2) {
		ShowClassLayout("DerivedB", (int*)this, sizeof(*this));
	}

	virtual void Function5(){
		cout<<"DerivedB::Function5()"<<endl;
	}

	virtual void Function6(){
		cout<<"DerivedB::Function6()"<<endl;
	}

	virtual ~DerivedB(){
		cout<<"DerivedB destroyed..."<<endl;
	}

};

class DerivedAB : public DerivedA, public DerivedB {
	int iMem;

public:
	DerivedAB(int aMem=0) : Base(aMem), 
		DerivedA(aMem),DerivedB(aMem), iMem(aMem+3) {
		ShowClassLayout("DerivedAB", (int*)this, sizeof(*this));
	}

	virtual void Function7(){
		cout<<"DerivedAB::Function7()"<<endl;
	}

	virtual void Function8(){
		cout<<"DerivedAB::Function8()"<<endl;
	}

	virtual ~DerivedAB(){
		cout<<"DerivedAB destroyed..."<<endl;
	}

};

void ThisPointerAlignment() {
	DerivedAB dab(20);
	Base* pBase = &dab;
	DerivedA* pA = &dab;
	DerivedB* pB = &dab;

	cout<<"Address of DerivedAB is : "<<&dab<<endl;
	cout<<"Address of Base within DerivedAB is : "<<pBase<<endl;
	cout<<"Address of DerivedA within DerivedAB is : "<<pA<<endl;
	cout<<"Address of DerivedB within DerivedAB is : "<<pB<<endl;
}

void DisplaySize() {
        cout<<"Displaying Size of All class : "<<endl;
        cout<<"Size of Base is : "<<sizeof(Base)<<endl;
        cout<<"Size of DerivedA is : "<<sizeof(DerivedA)<<endl;
        cout<<"Size of DerivedB is : "<<sizeof(DerivedB)<<endl;
        cout<<"Size of DerivedAB is : "<<sizeof(DerivedAB)<<endl<<endl;
}

void CheckVirtualLayout() {
	cout<<endl<<"**Base**"<<endl;
	Base base(20);
	cout<<endl<<"**DerivedA**"<<endl;
	DerivedA da(20);
	cout<<endl<<"**DerivedB**"<<endl;
	DerivedB db(20);
	cout<<endl<<"**DerivedAB**"<<endl;
	DerivedAB dab(20);
	cin.get();

	cout<<"Calling all virtual functions accessible for DerivedAB"<<endl;
	//calling virtual functions of DerivedAB (2) and DerivedA (2)
	long* ptr = (long*)&dab;
	CallVirtualFunction("DerivedAB", ptr, 4);
	//calling virtual functions of DerivedB (2)
	ptr += 3;
	CallVirtualFunction("DerivedAB", ptr, 2);
	//calling virtual functions of Base (2)
	ptr += 4;
	CallVirtualFunction("DerivedAB", ptr, 2);
}


#endif //VIRTUAL_H
