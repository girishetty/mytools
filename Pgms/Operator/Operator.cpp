#include <iostream>
using namespace std;

class Base {
private:
	int array [10];
	int size;

public:
	Base (int s = 10) {
		if(s > 10)
			s = 10;
		size = s;
		
		for(int i=0;i<size;i++)
			array[i] = i+1;
	}
	Base (Base& base) {
		size = base.size;
		for(int i=0;i<size;i++)
			array[i] = base.array[i];
	}

	int& operator[](int i) {
		return array[i];
	}
};

class Derived : public Base {
public :
	Derived (int s=10) : Base (s) {
	}
	Derived (Derived& d) : Base ((Base&) d) {
	}
};




//Access Control...
class A {
public:
	int f ();
	int g ;
private:
	int p ;
};

class B : private A {
public:
	A::f;  // Make public
	// A::p;  // Make public --> cant increae the visibility in derived class
protected:
	A::g ; // Make protected

};

class Measurement {
	int iMeter;
	int iCentimeter;

public:
	Measurement(int aMeter = 0, int aCentimeter = 0) : iMeter(aMeter), iCentimeter(aCentimeter) {}
	Measurement(const Measurement& aObj) : iMeter(aObj.iMeter), iCentimeter(aObj.iCentimeter) {}
	float Get() const { return iMeter + iCentimeter/100.0f; }
	//operator float() { return Get(); }
};


void OperatorTest() {
	Measurement v(10, 67);
	cout<<"Val is : "<<v.Get()<<endl;
	float f = v.Get();
	float f1 = v;
}

int main () {
	Derived d;
	Derived d1=d;

	cout<<d[4]<<endl;
	cout<<d1[5]<<endl;

	OperatorTest();
	return 0;
}