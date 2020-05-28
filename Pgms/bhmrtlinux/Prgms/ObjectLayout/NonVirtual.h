#ifndef NONVIRTUAL_H
#define NONVIRTUAL_H

#include "Header.h"

class NormalBase{
	int iMem;

public:
	NormalBase(int aMem=0) : iMem(aMem) {
		ShowClassLayout("NormalBase", (int*)this, sizeof(*this));
	}

	~NormalBase(){
		cout<<"NormalBase destroyed..."<<endl;
	}
};

class NormalDerivedA : virtual public NormalBase {
	int iMem;

public:
	NormalDerivedA(int aMem=0) : NormalBase(aMem), iMem(aMem+1) {
		ShowClassLayout("NormalDerivedA", (int*)this, sizeof(*this));
	}

	~NormalDerivedA(){
		cout<<"NormalDerivedA destroyed..."<<endl;
	}
};

class NormalDerivedB : virtual public NormalBase {
	int iMem;

public:
	NormalDerivedB(int aMem=0) : NormalBase(aMem), iMem(aMem+2) {
		ShowClassLayout("NormalDerivedB", (int*)this, sizeof(*this));
	}

	~NormalDerivedB(){
		cout<<"NormalDerivedB destroyed..."<<endl;
	}
};

class NormalDerivedAB : public NormalDerivedA, public NormalDerivedB {
	int iMem;

public:
	NormalDerivedAB(int aMem=0) : NormalBase(aMem), 
		NormalDerivedA(aMem),NormalDerivedB(aMem), iMem(aMem+3) {
		ShowClassLayout("NormalDerivedAB", (int*)this, sizeof(*this));
	}

	~NormalDerivedAB(){
		cout<<"NormalDerivedAB destroyed..."<<endl;
	}
};

void CheckNonVirtualLayout() {
	cout<<endl<<"**NormalBase**"<<endl;
	NormalBase base(20);
	cout<<endl<<"**NormalDerivedA**"<<endl;
	NormalDerivedA da(20);
	cout<<endl<<"**NormalDerivedB**"<<endl;
	NormalDerivedB db(20);
	cout<<endl<<"**NormalDerivedAB**"<<endl;
	NormalDerivedAB dab(20);
}

#endif //NONVIRTUAL_H

