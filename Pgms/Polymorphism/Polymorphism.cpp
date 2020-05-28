# include <iostream>
using namespace std;

class Base {

public: 
	Base () : memB (0) { }
	Base (int a) : memB (a) { }
	Base (Base& b) : memB (b.memB) { }

	virtual ~Base () { cout<<"~Base"<<endl; }

private:
	int memB;
};

class Derived : public Base {

public:
	Derived () : memD (0) { }
	Derived (int a, int b) : Base (a), memD (b) { }
	Derived (Derived& d) : Base ((Base&) d), memD (d.memD) { }

	virtual ~Derived () { cout<<"~Derived"<<endl; }

private:
	int memD;
};

void PointerAirthmatic () {
	int array [10];

	for(int i=0;i<10;i++)
		array [i] = i+1;

	int *p1 = array;
	int *p2[10];
	int (*p3)[10];

	p2[0] = p1;
	p3 = &array;

	unsigned long val1, val2;

	val1 = (unsigned long) p1;
	cout<<*p1<<endl;
	p1++;
	val2 = (unsigned long) p1;
	cout<<*p1<<endl;
	cout<<"difference of p1 is : "<<(val2-val1)<<endl;

	val1 = (unsigned long) *p2;
	cout<<**p2<<endl;
	(*p2)++;
	val2 = (unsigned long) *p2;
	cout<<**p2<<endl;
	cout<<"difference of p2 is : "<<(val2-val1)<<endl;

	val1 = (unsigned long) p3;
	cout<<**p3<<endl;
	p3++;
	val2 = (unsigned long) p3;
	cout<<**p3<<endl;
	cout<<"difference of p3 is : "<<(val2-val1)<<endl;
}

int main () {
	/*Base* pBase[10] = new Derived [10] ;
	delete [] pBase;*/

	PointerAirthmatic ();

	return 0;
}
