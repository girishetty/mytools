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
}; 

int main() { 
        Sample s1(10); 
        Sample s2 = s1; 
        Sample s3 = 10; 
        Sample s4 = Sample(10); 
		const Sample& s = s1;
		Sample s5 = s;

        return 0; 
}
