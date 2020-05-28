# include <iostream.h>

class FirstLevelBase {
public:
	virtual void virtualFunction () {
		cout<<"FirstLevelBase"<<endl;
	}
};

class SecondLevelBase : public FirstLevelBase {
public:
	void virtualFunction () {
		cout<<"SecondLevelBase"<<endl;
	}
};

class  ThirdLevelBase: public SecondLevelBase {
public:
	void virtualFunction () {
		cout<<"ThirdLevelBase"<<endl;
	}
};


class AbstractClass {

public:
	void ServiceL() {
		display();
	}
	
	virtual void display () = 0;
};

class SampleClass : public AbstractClass 
{
public:
	void display() {
	cout<<"Dervied";
	}
};

void virtualFunClass () {
	FirstLevelBase* pBase;
	pBase = new SecondLevelBase ();
	pBase->virtualFunction();
	delete pBase;

	pBase = new ThirdLevelBase ();
	pBase->virtualFunction();
	delete pBase;

	SecondLevelBase* pSBase;
	pSBase = new ThirdLevelBase ();
	pSBase->virtualFunction();
	delete pSBase;

	cout<<sizeof(FirstLevelBase)<<" : "<<sizeof(SecondLevelBase)<<" : "<<sizeof(ThirdLevelBase)<<endl;
}

class BaseClass
{
	public:
		BaseClass () { }
		virtual void Function () { Display(); }
	private:
		virtual void Display () { cout<<"Base : Display"<<endl; }
};

class DerivedClass : public BaseClass
{
	public: 
		DerivedClass () { }
	private:
		virtual void Display () { cout<<"Derived : Display"<<endl; }
};

void TestFunction ()
{
	BaseClass* pBase = new DerivedClass;
	pBase->Function();
}

void charTest() {
	char str[] = "Example";
	cout<<str<<endl;
	str[2] = 'A';
	cout<<str<<endl;
}

void unionTest() {
	union Example
	{
	int i;
	char c;
	};

	union Example ex;
	ex.i = 'A';
	cout<<ex.i<<endl;
	cout<<ex.c<<endl;

	ex.c = 'A';
	cout<<ex.i<<endl;
	cout<<ex.c<<endl;

	ex.i = 999;
	cout<<ex.i<<endl;
	cout<<ex.c<<endl;
}

void recursiveFun(int param) {
	cout<<param<<"  ";
	if(param >0) {
		recursiveFun (param-2);
	}
}

void incrementTest () {
	int i, j;
	j = (i=4, i++ * i++);
	cout<<j<<endl;
	j = (i=4, i++ * ++i);
	cout<<j<<endl;
	j = (i=4, ++i * i++);
	cout<<j<<endl;
	j = (i=4, ++i * ++i);
	cout<<j<<endl;
}


class X { 
public:
    X() : i(4) { this->foo(); }
    const int& get() const { return i; }

	virtual void baseFoo() { this->foo(); }
	virtual void foo() { cout<<"X::foo()"<<endl; }

private:     
	const int i;
};

class Y : public X { 
public:
	Y() : X(), i(5) { foo(); }
	const int& get() const { return i; }
	virtual void foo() { cout<<"Y::foo()"<<endl; }

private:      
	const int i;
};

void testXY () {
    X* x = new X;
    X* y = new Y;
    cout << "X-Y is : "<<(y->get()-x->get()) << endl;
	cout << "Size of Class X is : "<<sizeof(X)<<endl;
	cout << "Size of Class Y is : "<<sizeof(Y)<<endl;

	y->baseFoo();
	delete x;
    delete y;
}

// novtable 
class __declspec(novtable) NoVTableBase {
public:
	virtual void virtFun();
};

class NoVTableDerived: public NoVTableBase {
public:
	void virtFun () {
		cout<<"In Y"<<endl;
	}
};


void NoVTableTest () {
	NoVTableBase *p1 = new NoVTableBase();
	//p1->virtFun();   // AV at runtime
	NoVTableDerived *p2 = new NoVTableDerived();
	p2->virtFun();
	delete p2;
	delete p1;

	p1 = new NoVTableDerived();
	p1->virtFun();   // AV at runtime
	delete p1;
}

int main() {
	virtualFunClass();
	/*incrementTest ();

	AbstractClass * s = new SampleClass;
	
	s->ServiceL();

	TestFunction ();
	testXY (); 
	NoVTableTest ();*/
	return 0;
}
