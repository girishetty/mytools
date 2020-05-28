#include <iostream>
using namespace std;

#define VIRTUALLY	/*virtual*/
#define VIRTUAL		/*virtual*/
class Base{
	int iMem;

public:
	Base(int aMem = 0) : iMem(aMem) {
		cout<<"Base Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(Base));
	};

	VIRTUAL ~Base(){
		cout<<"Base Destructed..."<<endl;
	}

	void ObjectLayout(int* aPtr, int aLength){
		for(;aLength!=0;) {
			cout<<"At Address : "<<aPtr<<" Has : "<<*aPtr<<endl;
			aPtr++;
			aLength -= 4;
		}
		cout<<endl;
	}
};

//Non-Virtual inheritance
class Derived :  public Base {
	int iMem;
public:
	Derived(int aMem = 0) : Base(aMem), iMem(aMem+1)	{
		cout<<"Derived Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(Derived));
	};

	~Derived(){
		cout<<"Derived Destructed..."<<endl;
	}
};

//Virtual inheritance
class DerivedA : virtual public Base {
	int iMem;
public:
	DerivedA(int aMem = 0) : Base(aMem), iMem(aMem+1)	{
		cout<<"DerivedA Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(DerivedA));
	};

	~DerivedA(){
		cout<<"DerivedA Destructed..."<<endl;
	}
};

//Virtual inheritance 
class DerivedB : virtual public Base {
	int iMem;
public:
	DerivedB(int aMem = 0) : Base(aMem), iMem(aMem+1)	{
		cout<<"DerivedB Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(DerivedB));
	};
	~DerivedB(){
		cout<<"DerivedB Destructed..."<<endl;
	}
};

//Multiple inheritance of two Virtual base class
class JoinAB : public DerivedA, public DerivedB {
	int iMem;
public:
	JoinAB(int aMem = 0) : Base(aMem), DerivedA(aMem), DerivedB(aMem+1), iMem(aMem+3)	{
		cout<<"JoinAB Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(JoinAB));
	};
	~JoinAB(){
		cout<<"JoinAB Destructed..."<<endl;
	}
};


//Virtual inheritance with virtual functions
class DerivedAA : virtual public Base {
	int iMem;
public:
	DerivedAA(int aMem = 0) : Base(aMem), iMem(aMem+1)	{
		cout<<"DerivedAA Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(DerivedAA));
	};

	virtual void Function1()	{
		cout<<"DerivedAA::Function1"<<endl;		
	}
	virtual void Function2()	{
		cout<<"DerivedAA::Function2"<<endl;		
	}

	virtual ~DerivedAA(){
		cout<<"DerivedAA Destructed..."<<endl;
	}
};

//Virtual inheritance with virtual functions
class DerivedBB : virtual public Base {
	int iMem;
public:
	DerivedBB(int aMem = 0) : Base(aMem), iMem(aMem+1)	{
		cout<<"DerivedBB Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(DerivedBB));
	};
	virtual ~DerivedBB(){
		cout<<"DerivedBB Destructed..."<<endl;
	}
};

//Multiple inheritance of two Virtual base class and virtual functions
class JoinAABB : public DerivedAA, public DerivedBB {
	int iMem;
public:
	JoinAABB(int aMem = 0) : Base(aMem), DerivedAA(aMem), DerivedBB(aMem+1), iMem(aMem+3)	{
		cout<<"JoinAABB Constructed with Object Layout as : "<<endl;
		ObjectLayout((int*)this, sizeof(JoinAABB));
	};
	~JoinAABB(){
		cout<<"JoinAABB Destructed..."<<endl;
	}
};
