#include <iostream>
using namespace std;

class Sample {
	int iMem;

public:
	Sample(int aMem = 0) : iMem(aMem) {
		cout<<"Default/Int Constructor"<<endl;
	}

	Sample(const Sample& aSam) : iMem(aSam.iMem) {
		cout<<"Copy Constructor"<<endl;
	}

	Sample(Sample& aSam) : iMem(aSam.iMem) {
		cout<<"Overloaded Copy Constructor"<<endl;
	}

	Sample(Sample* aSam) : iMem(aSam->iMem) {
		cout<<"Overloaded Copy Constructor with ptr"<<endl;
	}
};

int main() {
	Sample s1(10);
	Sample s2 = s1;
	Sample s3 = 10;
	Sample s4 = Sample(10);
	Sample s5(&s1);
	Sample s6 = &s1;
	cout<<"Hi"<<endl;
	const Sample& s = s1;
	Sample s7 = s;

	return 0;
}

