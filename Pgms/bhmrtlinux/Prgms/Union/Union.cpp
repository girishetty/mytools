#include <iostream>
using namespace std;

class SampleClass{
	int mem;
public:
	//SampleClass () { }
	//~SampleClass () { }
	//SampleClass(SampleClass& uClass) { }
	//SampleClass& operator=(SampleClass& uClass) { }

	void Display () const {
		cout<<"SampleClass::Member Value is : "<<mem<<endl;
	}
	void SetMember (int a) {
		mem = a;
	}
};

union SampleUnion{
	int 	a;
	SampleClass s;
public:
	SampleUnion(){
		cout<<"SampleUnion constructed"<<endl;
	}
	~SampleUnion(){
		cout<<"SampleUnion destructed"<<endl;
	}

	void Display () const {
		cout<<"Integer Member Value is : "<<a<<endl;
		s.Display();
	}
};

int main() {
	SampleUnion sam;
	sam.a = 20;
	sam.Display();
	sam.s.SetMember(60);
	sam.Display();

	return 0;
}


