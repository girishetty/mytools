#include <iostream>
using namespace std;

class Sample {
public:
	explicit Sample(int a = 0) : mem(a) {
		cout<<"Constructing Sample with "<<a<<endl;
	}

	Sample& operator()(int a = 0){
		cout<<"Fun Operator "<<a<<endl;
		mem = a;
		return *this;
	}

	~Sample() {
		cout<<"Destructing Sample "<<mem<<endl;
	}
private:
	int mem;
};

class SampleOne {
public:
	SampleOne(int a = 0) : mem(a) {
		cout<<"Constructing SampleOne with "<<a<<endl;
	}

	SampleOne& operator()(int a = 0){
		cout<<"Fun Operator "<<a<<endl;
		mem = a;
		return *this;
	}

	~SampleOne() {
		cout<<"Destructing SampleOne "<<mem<<endl;
	}
private:
	int mem;
};

void DanglingRefOne() {
	cout<<"Start DanglingRefOne"<<endl;
	Sample* p = new Sample();
	Sample& sam = *p;
	sam();
	delete p;
	(*p)();
	sam();
	cout<<"End DanglingRefOne"<<endl;
	}

Sample& DanglingRefTwo() {
	cout<<"Start DanglingRefTwo"<<endl;
	Sample p;
	cout<<"End DanglingRefTwo"<<endl;
	return p;
	}

void ExplicitConst() {
	Sample sam;
	Sample sam1(1);
	//Sample sam2 = 1;  Error!
	//sam2 = 56;        Error!

	cout<<"Done"<<endl;
	}

void TempObj() {
	cout<<"Start TempObj"<<endl;
	SampleOne sam = 'c';
	SampleOne sam1('d');

	sam1 = 'f';
	cout<<"End TempObj"<<endl;
	}

void FunObj() {
	Sample sam;
	sam()(8);
}

int main() {
	ExplicitConst();
	FunObj();
	TempObj();
	DanglingRefOne();
	DanglingRefTwo();

	return 0;
}


