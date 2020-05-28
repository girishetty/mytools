/****************************************************
 Size of Each class in Virtual Inheritance Hierarchy
 ****************************************************/

class Base{
public:
	virtual ~Base() { }
};

class DereivedA : public virtual Base {
};

class DereivedB : public virtual Base {
};

class Dereived : public DereivedA, public DereivedB {
};

class DereivedC : public Base {
};


/***********************************************************************************
 Demo of Sequence of Base Class Construction Call in a Virtual Inheritance Hierarchy
 ***********************************************************************************/
class NewBase{
	int iMem;

public:
	NewBase(int i) : iMem(i){
	cout<<"NewBase "<<iMem<<endl;
	}
	int Display() {
		return iMem;
	}
};

class ABase : public NewBase {
	int iMem;

public:
	ABase(int i) : NewBase(i), iMem(i){
	cout<<"ABase "<<iMem<<endl;
	}
	int Display() {
		return iMem;
	}
};


class BBase : public virtual ABase {
	int iMem;

public:
	BBase(int i) : ABase(i+1), iMem(i) {
	cout<<"BBase "<<iMem<<endl;
	}
};

class CBase : public virtual ABase {
	int iMem;

public:
	CBase(int i) : ABase(i+2), iMem(i) {
	cout<<"CBase "<<iMem<<endl;
	}
};

class ABCDerived : public BBase, public CBase {
	int iMem;

public:
	ABCDerived(int i) : ABase(i+5), BBase(i+3), CBase(i+4), iMem(i) {
	cout<<"ABCDerived "<<iMem<<endl;
	}
	int Display(){
		return ABase::Display();
	}
};




/*************************************************************************************
 Implementing Final Class,
 Suppose Usage is ur Class and if u dont want others to inherit their class from ur 
 Usage class, implement it this way
 *************************************************************************************/
class Usable;

class Usable_lock {
	friend class Usable;

private:
	Usable_lock() {}
	Usable_lock(const Usable_lock&) {}
	};

class Usable : public /*virtual*/ Usable_lock {
public:
	Usable() {}
	Usable(char*);
	/*  All ur implementation of Usage class Comes here*/
	};

class DD  : public Usable {// Its a compiler Error, if u try to inherit from Usage!!!
public:
	DD() { }
};
	
