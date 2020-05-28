# include <iostream.h>
# include <string.h>

// Some Questions on Empty class and complier Optimizations
class Empty {
};

class DerivedEmpty : public Empty {
};

class Derived : public DerivedEmpty {
public:
	int a;
};

void Function (Derived* pDerived) {
	void* p1 = pDerived;
	void* p2 = &pDerived->a;
	if(p1==p2) {
		cout<<"Oops...!!!"<<endl;
	}
	else {
		cout<<"Ailaa...!!!"<<endl;
	}
}
void EmptyClassTest () {
	cout<<"Size of Empty		: "<<sizeof(Empty)<<endl;
	cout<<"Size of DerivedEmpty : "<<sizeof(DerivedEmpty)<<endl;
	cout<<"Size of Derived		: "<<sizeof(Derived)<<endl;

	Derived* pDerived = new Derived;
	pDerived->a = 10;

	Function(pDerived);
}

//Overloading base class function in derived

class BaseWithOneFun {
public:
	BaseWithOneFun () {
		cout << "BaseWithOneFun"<<endl;
	}
	int f(int i) { cout << "f(int): "; return i+1; }
};

class DerivedWithOneFun : public BaseWithOneFun {
public:
	using BaseWithOneFun::f;
	double f(double d) { cout << "f(double): "; return d+0.3; }
};

void OverLoadingInDerived () {
	DerivedWithOneFun* p = new DerivedWithOneFun;
	cout<<p->f(2)<<endl;
	cout<<p->f(2.3)<<endl;
}

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
	
void FinalClassTest () {
	DD d;
}

class ABase{
	int iMem;

public:
	ABase(int i) : iMem(i){
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


void testVirtualInheritance() {
	cout<<"Testing Virtual Inheritance"<<endl;
	cout<<"Size of ABase : "<<sizeof(ABase)<<endl;
	cout<<"Size of BBase : "<<sizeof(BBase)<<endl;
	cout<<"Size of CBase : "<<sizeof(CBase)<<endl;
	cout<<"Size of ABCDerived : "<<sizeof(ABCDerived)<<endl;
	ABCDerived d(7);
}

class Sample {
public:
	Sample () : mem (0) { cout<<"Sample"<<endl;
	}
	Sample (int a) : mem (a) { cout<<"Sample"<<endl;
	}
	~Sample () { cout<<"~Sample"<<endl;
	}

	Sample& operator=(Sample& sam) {
		return *this;
	}
private:
	const int mem;
};

void ClassFunctionTest () {
	// Both are function declarations
	Sample sam();
	Sample* psam();

	//delete psam;
}


class A {
	int a;
	char c;
};

class B : public A {
	char d;
	int i;
	static double stat;
};

class C {
	int int1;
	int int2;
	int i;
	long l;
	short s;
	char c;
};

class ClassA {
};
 
class ClassB{
};
 
class ClassC: public ClassA, public ClassB {
private:
int a;
};
 

void ClassSizeTestOne () {
	cout<<"Testing Class Size"<<endl;
	cout<<"Size of A is : "<<sizeof(A)<<endl;
	cout<<"Size of B is : "<<sizeof(B)<<endl;
	cout<<"Size of C is : "<<sizeof(C)<<endl;
	cout<<"Size of ClassA is : "<<sizeof(ClassA)<<endl;
	cout<<"Size of ClassB is : "<<sizeof(ClassB)<<endl;
	cout<<"Size of ClassC is : "<<sizeof(ClassC)<<endl;
}

void constMemClassTest () {
	Sample sam1(4), sam2(5);
	sam1 = sam2;
}


class FunctionObjClass {

public:
	FunctionObjClass (int val = 10) { iVal = val; }
	int operator()() const { return iVal; }
private:
	int iVal;

};

void FunctionObjTest () {
	FunctionObjClass obj(30);
	cout<<obj()<<endl;
}

void GeneralPtrFunction (void* ptr, void* pVal) {
	ptr = pVal;
	//ptr ++;
}

void PointerAirthmaticTest () {
	char  charArray[20];
	int   intArray[20];
	float floatArray[20];

	char  c = 'C';
	int   i = 10;
	float f = 12.0f;
	GeneralPtrFunction (charArray, &c);
	GeneralPtrFunction (intArray, &i);
	GeneralPtrFunction (floatArray, &f);
}

// This function does delete operation and assigns 0 to the pointer to avoid
// Double deletion problem

template<class Type> inline void Destroy(Type*& aPtr) {
	delete aPtr;
	cout<<"Pointer : "<<aPtr<<" is deleted "<<endl;
	aPtr = 0;
}

void DoubleDeletionTest() {
	int* p = new int;
	Destroy(p); //delete p;
	Destroy(p);  //delete p;
}


void SwapFunTest () {
	int x=10;
	int y=20;
	cout<<"X is : "<<x<<"  Y is : "<<y<<endl;

	x = x + y;
	y = x - y;
	x = x - y;
	cout<<"X is : "<<x<<"  Y is : "<<y<<endl;

	x ^= y ^= x ^= y;
	cout<<"X is : "<<x<<"  Y is : "<<y<<endl;
}

void ExceptionTest (int i) {
	cout<<"Started ExceptionTest "<<endl;
	try {
		if(i == 10)
			throw i;
	}
	catch(int i) {
		cout<<"Exception Occured "<<i<<endl;
	}
	cout<<"Exiting ExceptionTest "<<endl;
}

class OverLoadClass {
	enum {
		ESize = 100
	};

public:
	OverLoadClass () : iStr(NULL) { }
	OverLoadClass (const char* aStr) {
		CopyString(aStr);
	}

	OverLoadClass (const OverLoadClass& aObj) {
		CopyString(aObj.iStr);
	}

	OverLoadClass& operator= (const char* aStr) {
		if(this->iStr != aStr) {
			delete [] iStr;
			CopyString(aStr);
		}
		return *this;
	}

	OverLoadClass& operator= (const OverLoadClass& aObj) {
		if( this != &aObj) {
			delete [] iStr;
			CopyString(aObj.iStr);
		}
		return *this;
	}

	~OverLoadClass() {
		delete [] iStr;
	}

	void Display() {
		cout<<"String is : "<<iStr<<endl;
	}

private:

	void CopyString(const char* aStr) {
		iStr = new char [ESize];
		strcpy(iStr, aStr);
	}

private:
	char* iStr;
};

void OverLoadClassTest () {
	OverLoadClass obj1("Girish");
	OverLoadClass obj2(obj1);
	obj1.Display();
	obj2.Display();
	obj1 = obj2;
	obj1 = obj1;
	obj1.Display();
}

class ClassZero{
	char	charVal;
	char 	charVal1;
	short	shortVal;
	int		intVal;
};

class ClassOne{
	char	charVal;
	int		intVal;
	short	shortVal;
	char 	charVal1;
};

class ClassTwo{
	char	charVal;
	short	shortVal;
	int		intVal;
	char 	charVal1;
};

class ClassThree{
	char	charVal;
	short	shortVal;
	int		intVal;
	float	floatVal;
	long double	doubleVal;
};

class ClassFour{
	char	charVal;
	short	shortVal;
	int		intVal;
	long double	doubleVal;
	float	floatVal;
};

class ClassFive{
	char	charVal;
	short	shortVal;
	static char	charStatVal;
	short	shortVal2;
};

void ClassSizeTestTwo () {
	cout<<"Size of ClassZero is : "<<sizeof(ClassZero)<<endl;
	cout<<"Size of ClassOne is : "<<sizeof(ClassOne)<<endl;
	cout<<"Size of ClassTwo is : "<<sizeof(ClassTwo)<<endl;
	cout<<"Size of ClassThree is : "<<sizeof(ClassThree)<<endl;
	cout<<"Size of ClassFour is : "<<sizeof(ClassFour)<<endl;
	cout<<"Size of ClassFive is : "<<sizeof(ClassFive)<<endl;
}


void ReferenceTest () {
	int i = 20;
	int j = 30;
	int& ref = i;
	cout<<"i has : "<<i<<endl;
	cout<<"j has : "<<j<<endl;
	cout<<"ref has : "<<ref<<endl;
	ref = j;
	cout<<"i has : "<<i<<endl;
	cout<<"j has : "<<j<<endl;
	cout<<"ref has : "<<ref<<endl;
	ref = 40;
	cout<<"i has : "<<i<<endl;
	cout<<"j has : "<<j<<endl;
	cout<<"ref has : "<<ref<<endl;
	j = 80;
	cout<<"i has : "<<i<<endl;
	cout<<"j has : "<<j<<endl;
	cout<<"ref has : "<<ref<<endl;
}

class VirtualDestBase{
public:
	VirtualDestBase () { cout<<"VirtualDestBase"<<endl; }
	virtual ~VirtualDestBase () { cout<<"~VirtualDestBase"<<endl; }
};

class VirtualDestDerived : public VirtualDestBase{
	int a;

public:
	VirtualDestDerived () { cout<<"VirtualDestDerived"<<endl; }
	~VirtualDestDerived () { cout<<"~VirtualDestDerived"<<endl; }
};

void VirtualDestTest () {
	VirtualDestBase* pBase = new VirtualDestDerived [5];
	delete [] pBase;
}

void EnumTest () {
	enum Sample { val1, val2, val3=6, val4 };
	int int1 = val1;
	int int4 = val4;
	cout<<"Val1 and Val4 are : "<<int1<<" : "<<int4<<endl;
}

/*
Union Restriction Test
*/

void UnionTest() {
	class SampleClass{
		int mem;
	public:
		//SampleClass () { }
		//~SampleClass () { }
		//SampleClass(SampleClass& uClass) { }
		//SampleClass& operator=(SampleClass& uClass) { }

		void Display () const {
			cout<<"Member Value is : "<<mem<<endl;
		}
		void SetMember (int a) {
			mem = a;
		}
	};

	union SampleUnion{
		int			a;
		SampleClass s;

		void Display () const {
			cout<<"Integer Member Value is : "<<a<<endl;
			cout<<"Class SampleClass ";
			s.Display();
		}
	};
	
	SampleUnion sam;
	sam.a = 20;
	sam.Display();
	sam.s.SetMember(60);
	sam.Display();
}

void ShiftOperation () {
	unsigned int shiftCount = 8;
	unsigned long num = 0x2020;
	unsigned long num1 = (num >> shiftCount) << shiftCount ;
	cout<<num1<<endl;
}


/*
Calling Constructer Explicitly in a constructer
*/

class OneClass {
	int a;
public:
	OneClass (int val = 0) { 
		if(a>0)
			a+=val;
		else
			a=val;
	}
	void Set (int val) { a = val; }
	int Get () { return a; }
};

class TwoClass {
	OneClass one;
public:
	TwoClass (int val = 0) : one(val) {
		one.OneClass::OneClass(val+10);
	}
	void Set (int val) { one.Set (val); }
	int Get () { return one.Get (); }
};

void ExplicitConstTest () {
	TwoClass two(5);
	cout<<"Value is : "<<two.Get()<<endl;
}

/*
Usage of delete [] (at a stretch) and delete (one by one) on Pointer Array
*/

class BaseOne {
public:
	virtual ~BaseOne() {cout<<"~BaseOne"<<endl; }
};

class DerivedOne : public BaseOne {
public:
	virtual ~DerivedOne() {cout<<"~DerivedOne"<<endl; }
};

void DeleteUsingBaseClass (int option) {
	const unsigned int KSize = 2;
	int i;
	cout<<"DeleteUsingBaseClass ( "<<option<<" )"<<endl;
	
	if(option & 0x1 ) {
		cout<<"Deleting BaseOne* "<<endl;
		BaseOne* pObj1 = new DerivedOne [KSize]; // Result is same even if we change the statement as DerivedOne* pObj1
		if(option == 1) {
			cout<<"Deleting All at once"<<endl;
			delete [] pObj1;
		}
		else {
			//This way of Deleting CRASHES !!
			cout<<"Deleting One-By-One"<<endl;
			for(int i=0; i<KSize; i++) {
				delete &(pObj1[i]);
				/* Below statement rises ERROR:
				'delete' : cannot convert from 'class BaseOne' to ''
				No user-defined-conversion operator available that can perform this conversion, or the operator cannot be called
				delete pObj1[i];
				*/
			}
		}
	}
	else{
		cout<<"Deleting BaseOne* []"<<endl;
		BaseOne* pObj2[KSize];  // Result is same even if we change the statement as DerivedOne* pObj2
		for(i=0; i<KSize; i++) {
			pObj2[i] = new DerivedOne;
		}
		if(option == 2) {
			cout<<"Deleting One-By-One"<<endl;
			for(i=0; i<KSize; i++) {
				delete pObj2[i];
			}
		}
		else {
			// This way of Deleting CRASHES !!
			cout<<"Deleting All at once"<<endl;
			delete [] &pObj2;
			/* Below statement rises WARNING:
			deletion of an array expression; conversion to pointer supplied
			delete [] pObj2;
			*/
		}
	}
}

void DeleteUsingBaseClassTest () { 
	DeleteUsingBaseClass (1);
	DeleteUsingBaseClass (2);
	DeleteUsingBaseClass (3);
	DeleteUsingBaseClass (4);
}

/*
Illustration of Member Templates..
By using a template constructor it is possible to construct a Pair from any other Pair, 
assuming that conversion from NewTypeOne to TypeOne and NewTypeTwo to TypeTwo are supported. 
Without the availability of template constructors one could only declare constructors with 
fixed types like "Pair(int)" or else use the template arguments to Pair itself, 
as in "Pair(TypeOne, TypeTwo)". 
*/
template <class TypeOne, class TypeTwo> struct Pair {
public:
	TypeOne iFirstMem;
	TypeTwo iSecondMem;

public:
	Pair(const TypeOne& ax, const TypeTwo& bx) : iFirstMem(ax), iSecondMem(bx) {}

	template <class NewTypeOne, class NewTypeTwo> 
		Pair(const Pair<NewTypeOne, NewTypeTwo>& aPair): 
		iFirstMem(aPair.iFirstMem), iSecondMem(aPair.iSecondMem) {}

	void Display() const {
		cout<<"First  Member is : "<<iFirstMem<<endl;
		cout<<"Second Member is : "<<iSecondMem<<endl;
	}
};

void MemberTemplateTest () {
	//Create a Pair with "double and long double"
	Pair<double, long double> pair1(37.67, 12.34);
	//Create another Pair with "double and long double" using previous Pair
	Pair<short, double> pair2(pair1);
	pair1.Display();
	pair2.Display();
}

int main() {
	//EmptyClassTest ();
	//OverLoadingInDerived ();
	//FinalClassTest ();
	//ClassFunctionTest ();
	ClassSizeTestOne ();
	//FunctionObjTest ();
	//PointerAirthmaticTest ();
	//DoubleDeletionTest ();
	//SwapFunTest ();
	//ExceptionTest (10);
	//OverLoadClassTest ();
	//ClassSizeTestTwo ();
	//ReferenceTest ();
	//VirtualDestTest ();
	//EnumTest ();
	//UnionTest();
	//ShiftOperation ();
	//ExplicitConstTest ();
	//DeleteUsingBaseClassTest ();
	//MemberTemplateTest ();
	testVirtualInheritance ();

	return 0;
}
