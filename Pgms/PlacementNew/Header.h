#ifndef HEADER_H
#define HEADER_H

#include <iostream>
using namespace std;

class MyClass {
public:
	MyClass() : iObjId (++iObjCount) { 
		cout<<"Constructing MyClass Object : "<<iObjId<<endl;
	}

	~MyClass() { 
		cout<<"Destructing MyClass Object : "<<iObjId<<endl;
	}
public:
	static int iObjCount;
private:
	const int  iObjId;
};

#endif //HEADER_H
