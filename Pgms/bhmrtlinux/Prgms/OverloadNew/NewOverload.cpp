#include <stddef.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Sample {
private:
public:
	Sample() {
		cout<<"Sample::Sample"<<endl;
	}
	~Sample() {
		cout<<"Sample::~Sample"<<endl;
	}

public:
	inline void* operator new(size_t aSize);     
	inline void operator delete(void* aPtr);
};

inline void* Sample::operator new(size_t aSize) {
	cout<<"Sample::operator new"<<endl;
	return malloc(aSize);
}
inline void Sample::operator delete(void* aPtr) {
	cout<<"Sample::operator delete"<<endl;
	free(aPtr);
}

inline void* operator new(size_t aSize) {
	cout<<"::operator new"<<endl;
	return malloc(aSize);
}
inline void operator delete(void* aPtr) {
	cout<<"::operator delete"<<endl;
	free(aPtr);
}

int main(){
	Sample* pSam = new Sample;
	delete pSam;
	int* p = new int;
	delete p;
	return 0;
}

