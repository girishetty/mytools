#ifndef CLASSDEF_H
#define CLASSDEF_H

#include <iostream>
using namespace std;

class BaseMost {

public:
	BaseMost(int aMem = 0) : iMem(aMem) {
		cout<<"BaseMost::BaseMost --> iMem is "<<iMem<<endl;
	}

	virtual ~BaseMost() {
		cout<<"BaseMost::~BaseMost --> iMem is "<<iMem<<endl;
	}

	void MemberAddressBM() const {
		cout<<"Address of BaseMost::iMem is : "<<&iMem<<endl;
	}

private:
	int iMem;
};

class Base : public BaseMost {

public:
	Base(int aMem = 0) : BaseMost(aMem+1), iMem(aMem) {
		cout<<"Base::Base --> iMem is "<<iMem<<endl;
	}

	virtual ~Base() {
		cout<<"Base::~Base --> iMem is "<<iMem<<endl;
	}

	void MemberAddressB() const {
		cout<<"Address of Base::iMem is : "<<&iMem<<endl;
	}
private:
	int iMem;
};

class DerivedA : virtual public Base {

public:
	DerivedA(int aMem = 0) : Base(aMem+1), iMem(aMem) {
		cout<<"DerivedA::DerivedA --> iMem is "<<iMem<<endl;
	}

	virtual ~DerivedA() {
		cout<<"DerivedA::~DerivedA --> iMem is "<<iMem<<endl;
	}

	void MemberAddressDA() const {
		cout<<"Address of DerivedA::iMem is : "<<&iMem<<endl;
	}
private:
	int iMem;
};

class DerivedB : virtual public Base {

public:
	DerivedB(int aMem = 0) : Base(aMem+1), iMem(aMem) {
		cout<<"DerivedB::DerivedB --> iMem is "<<iMem<<endl;
	}

	virtual ~DerivedB() {
		cout<<"DerivedB::~DerivedB --> iMem is "<<iMem<<endl;
	}

	void MemberAddressDB() const {
		cout<<"Address of DerivedB::iMem is : "<<&iMem<<endl;
	}
private:
	int iMem;
};

class DerivedAB : virtual public DerivedA, virtual public DerivedB {

public:
	DerivedAB(int aMem = 0) : Base(aMem+3), DerivedA(aMem+2), DerivedB(aMem+1), iMem(aMem) {
		cout<<"DerivedAB::DerivedAB --> iMem is "<<iMem<<endl;
	}

	virtual ~DerivedAB() {
		cout<<"DerivedAB::~DerivedAB --> iMem is "<<iMem<<endl;
	}

	void MemberAddressDAB() const {
		cout<<"Address of DerivedAB::iMem is : "<<&iMem<<endl;
	}
private:
	int iMem;
};

class Derived : public DerivedAB {

public:
	Derived(int aMem = 0) : Base(aMem+4), DerivedA(10), DerivedAB(aMem+1), iMem(aMem) {
		cout<<"Derived::Derived --> iMem is "<<iMem<<endl;
	}

	virtual ~Derived() {
		cout<<"Derived::~Derived --> iMem is "<<iMem<<endl;
	}

	void MemberAddressD() const {
		cout<<"Address of Derived::iMem is : "<<&iMem<<endl;
	}
private:
	int iMem;
};


class VBase{
public:
	virtual void DisplayA(bool aBool=true) = 0;
	virtual void DisplayB(bool aBool=true) = 0;
	virtual ~VBase(){
		cout<<"~VBase"<<endl;
	}
};

class Derived1 : virtual public VBase {
public:
	virtual void DisplayA(bool aBool=true){
		cout<<"Derived1::DisplayA"<<endl;
		if(aBool){
			DisplayB(false);
		}
	}
	virtual ~Derived1(){
		cout<<"~Derived1"<<endl;
	}
};

class Derived2 : virtual public VBase {
public:
	virtual void DisplayB(bool aBool=true){
		cout<<"Derived2::DisplayB"<<endl;
		if(aBool){
			DisplayA(false);
		}
	}
	virtual ~Derived2(){
		cout<<"~Derived2"<<endl;
	}
};

class Join : public Derived1, public Derived2 {
public:
	virtual ~Join(){
		cout<<"~Join"<<endl;
	}
};

/*********
 More Sample codes 
 **********/

class A {
public:
	virtual ~A() { }
};

class B {
public:
	virtual ~B() { }
};


class C {
public:
	//virtual ~C() { }
};

class D : public virtual A, public virtual B, public virtual C {
//class D : public A, public B, public C {
public:
	//virtual ~D() { }
};

#endif // CLASSDEF_H
