#ifndef CLASSDEF_H
#define CLASSDEF_H

#include <iostream>
using namespace std;

class BaseMost {

public:
	BaseMost(int aMem = 0) : iMem(aMem) {
		cout<<"BaseMost::BaseMost --> iMem is "<<iMem<<endl;
	}

	~BaseMost() {
		cout<<"BaseMost::~BaseMost --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

class Base : public BaseMost {

public:
	Base(int aMem = 0) : BaseMost(aMem+1), iMem(aMem) {
		cout<<"Base::Base --> iMem is "<<iMem<<endl;
	}

	~Base() {
		cout<<"Base::~Base --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

class DerivedA : virtual public Base {

public:
	DerivedA(int aMem = 0) : Base(aMem+1), iMem(aMem) {
		cout<<"DerivedA::DerivedA --> iMem is "<<iMem<<endl;
	}

	~DerivedA() {
		cout<<"DerivedA::~DerivedA --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

class DerivedB : virtual public Base {

public:
	DerivedB(int aMem = 0) : Base(aMem+1), iMem(aMem) {
		cout<<"DerivedB::DerivedB --> iMem is "<<iMem<<endl;
	}

	~DerivedB() {
		cout<<"DerivedB::~DerivedB --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

class DerivedAB : public DerivedA, public DerivedB {

public:
	DerivedAB(int aMem = 0) : /*Base(aMem+3),*/ DerivedA(aMem+2), DerivedB(aMem+1), iMem(aMem) {
		cout<<"DerivedAB::DerivedAB --> iMem is "<<iMem<<endl;
	}

	~DerivedAB() {
		cout<<"DerivedAB::~DerivedAB --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

class Derived : public DerivedAB {

public:
	Derived(int aMem = 0) : /*Base(aMem+4), */ DerivedAB(aMem+1), iMem(aMem) {
		cout<<"Derived::Derived --> iMem is "<<iMem<<endl;
	}

	~Derived() {
		cout<<"Derived::~Derived --> iMem is "<<iMem<<endl;
	}
private:
	int iMem;
};

#endif // CLASSDEF_H