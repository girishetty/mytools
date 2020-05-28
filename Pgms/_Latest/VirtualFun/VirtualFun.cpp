#include <iostream>
using namespace std;


class BaseMost {
public:
	BaseMost() {
		cout<<"BaseMost constructing.."<<endl;
		Display();
		//This is a linking ERROR, we cant call pure virtual function from constructor
		//Function();
	}
	virtual ~BaseMost() {
		cout<<"BaseMost destructing.."<<endl;
		Display();
		//This is a linking ERROR, we cant call pure virtual function from destructor
		//Function();
	}
	virtual void Display() const {
		cout<<"BaseMost::Display.."<<endl;
	}
	void BaseMostFunction() const {
		cout<<"BaseMost::BaseMostFunction.."<<endl;
		Display();
		Function();
	}
	void virtual Function() const = 0;
};

class Base : public BaseMost {
public:
	Base() {
		cout<<"Base constructing.."<<endl;
		Display();
	}
	~Base() {
		cout<<"Base destructing.."<<endl;
		Display();
	}
	void Display() const {
		cout<<"Base::Display.."<<endl;
	}
	void BaseFunction() const {
		cout<<"Base::BaseFunction.."<<endl;
		Display();
	}
	void virtual Function() const {
		cout<<"Base::Function.."<<endl;
	}
};

class Derived : public Base {
public:
	Derived() {
		cout<<"Derived constructing.."<<endl;
		Display();
	}
	~Derived() {
		cout<<"Derived destructing.."<<endl;
		Display();
	}
	void Display() const {
		cout<<"Derived::Display.."<<endl;
	}
	void DerivedFunction() const {
		cout<<"Derived::DerivedFunction.."<<endl;
		Display();
	}
	void virtual Function() const {
		cout<<"Derived::Function.."<<endl;
	}
};

void ConstructionDestruction() {
	cout<<"Construct Derived with BaseMost"<<endl;
	BaseMost* pBaseMost = new Derived;
	cout<<endl<<"Calling VF fron BaseMost NVF"<<endl;
	pBaseMost->BaseMostFunction();
	cout<<endl<<"Destroy Derived with BaseMost"<<endl;
	delete pBaseMost;

	cout<<endl<<"Construct Derived with Base"<<endl;
	Base* pBase = new Derived;
	cout<<endl<<"Calling VF fron Base NVF"<<endl;
	pBase->BaseFunction();
	cout<<endl<<"Destroy Derived with Base"<<endl;
	delete pBase;
}

int main(){
	ConstructionDestruction();

	return 0;
}