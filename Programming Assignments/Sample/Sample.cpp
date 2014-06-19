# include <iostream>
using namespace std;

class Sample {
public:
	template<typename Type> friend void WorkOnThis(Type& aType);
	Sample(int aVal=0) : iVal(aVal) { }
	~Sample() { }
	void Display () const { cout<<"Value is : "<<iVal<<endl; }
	/* But below statement wont work with Microsoft C++ Compiler !!
	template<typename Type> friend class ChangeThis; */

private:
	int iVal;
};

template<typename Type> void WorkOnThis(Type& aType) {
	aType.iVal = 200;
}


void TestTemplateBasedFrnd() {
	Sample s1;
	Sample s2(10);

	s1.Display();
	s2.Display();

	WorkOnThis(s1);
	s1.Display();
}

void TestPtr() {
	int a[ ][4] = {{1,2,3,4},{5,6,7,8}};
	cout<<"Value of *(a[1]+2) is : "<<*(a[1]+2)<<endl;
}

class DataBuffer {
	enum { bufferSize=0x20000 };
	unsigned char buffer_[bufferSize];
public:
	unsigned char * const Buffer() const {
	return buffer_;
	};
};


int main () {
	//TestTemplateBasedFrnd();
	TestPtr();
	return 0;
}

