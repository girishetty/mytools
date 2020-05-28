#include <iostream>
using namespace std;

#define COMMENT

int globVal = 1;

class Base {
public:
	int iMem1;
	int iMem2;

	Base(int aVal = globVal++) : iMem1(aVal), iMem2(aVal+1) {
		cout<<"Base::Base"<<endl;
	}

	virtual ~Base(){
		cout<<"Base::~Base"<<endl;
	}

	virtual void Diaplay() {
		cout<<"Base::Display: iMem1 and iMem2 are : "<<iMem1<<" "<<iMem2<<endl;
	}
};

class Derived : public Base {
public:
	int iMem3;
	int iMem4;

	Derived(int aVal = globVal++) : Base(aVal), iMem3(aVal+2), iMem4(aVal+3){
		cout<<"Derived::Derived"<<endl;
	}
	
	virtual ~Derived(){
		cout<<"Derived::~Derived"<<endl;
	}

	virtual void Diaplay() {
		Base::Diaplay();
		cout<<"Derived::Display: iMem3 and iMem4 are : "<<iMem3<<" "<<iMem4<<endl;
	}
};

void ArrayWithPolymorphismOne() {
	cout<<endl<<"Constructing.."<<endl;
	Derived* ptr = new Derived[2];

	cout<<endl<<"Displaying.."<<endl;
	cout<<"Diaplaying 1st one"<<endl;
	ptr[0].Diaplay();
	cout<<"Diaplaying 2nd one"<<endl;
	ptr[1].Diaplay();

	cout<<endl<<"Distructing.."<<endl;
	delete []ptr;
} 

void ArrayWithPolymorphismTwo() {
	cout<<endl<<"Constructing.."<<endl;
	Derived* ptr = new Derived[2];
	Base* pBase = ptr;

#ifndef COMMENT
	cout<<endl<<"Displaying.."<<endl;
	cout<<"Diaplaying 1st one"<<endl;
	pBase->Diaplay();
	cout<<"Displaying 2nd one"<<endl;
	(++pBase)->Diaplay();
#endif //COMMENT

	cout<<endl<<"Distructing.."<<endl;
	delete []ptr;
} 

void ArrayWithPolymorphismThree() {
	cout<<endl<<"Constructing.."<<endl;
	Base* ptr = new Derived[2];

#ifndef COMMENT
	cout<<endl<<"Displaying.."<<endl;
	cout<<"Diaplaying 1st one"<<endl;
	ptr[0].Diaplay();
	cout<<"Diaplaying 2nd one"<<endl;
	ptr[1].Diaplay();
#endif //COMMENT

	cout<<endl<<"Distructing.."<<endl;
	delete []ptr;
} 

int main() {
	/*cout<<"Array With Polymorphism Test Case 1"<<endl;
	ArrayWithPolymorphismOne();
	cout<<"Array With Polymorphism Test Case 2"<<endl;
	ArrayWithPolymorphismTwo();
	cout<<"Array With Polymorphism Test Case 3"<<endl;*/
	ArrayWithPolymorphismThree();

	return 0;
}


