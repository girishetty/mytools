#include <iostream>
using namespace std;

class Derived;
//void Derived::SetVal(int aVal);

class Base {

public :
	void SetVal(int aVal ) {
		//Derived::SetVal(aVal);
	}

	virtual ~Base() { }
};

class Derived : public Base {
public :
	void SetVal(int aVal) {
		iVal = aVal;
	}

private:
	int iVal;
};

class SampleBase {
public:
	virtual ~SampleBase() { }
	virtual void SetData(int aMem) { }
};

class Sample : public SampleBase {
private:
	int iMem;

public :
	void DisplayData() {
		cout<<"Data is : "<<iMem<<endl;
	}

	Sample (int aMem = 1) : iMem(aMem) { }
private:
        void SetData(int aMem) {
                iMem = aMem;
	}
};

void TestAccessFun() {
	Base* p = new Derived;
	p->SetVal(10);
	delete p;
}

void ModifyMemberData(void* aPtr, int aVal) {
	int* ptr = (int*) aPtr;
	*ptr = aVal;
}
void ModifyUsingVirtual(SampleBase* aBase, int aVal) {
	aBase->SetData(aVal);
}

void CreateAndModify() {
	/*
	//solution #1 : Bad way of doing... Will be screwed up at times :-)
	Sample* p = new Sample();
	p->DisplayData();
	ModifyMemberData(p, 20);
	p->DisplayData();
	delete p;
	*/

	//solution #2 : with assignment operator :-)
	Sample s1;
	s1.DisplayData();
	Sample s2(20);
	s1 = s2;
	s1.DisplayData();

	//solution #3 : with placement new operator :-)
	Sample s3;
	s3.DisplayData();
	Sample* p3 = new (&s3) Sample (40);
	s3.DisplayData();

	//solution #4 : using virtual functions
	Sample s4;
	s4.DisplayData();
	ModifyUsingVirtual(&s4, 50);
	s4.DisplayData();
}

int main() {
	CreateAndModify();
	return 0;
}




