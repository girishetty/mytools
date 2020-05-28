#include <iostream>
using namespace std;

class DataBuffer {
	enum { EBufferSize = 100 };
	unsigned char iBuffer[EBufferSize];

public:
	unsigned char * const Buffer() {
		return iBuffer;
	}

	//prototype should have leading const, else its an error !!
	const unsigned char * const Buffer() const {
		return iBuffer;
	}
};
class X { 
private: 
	const int i;
public:
	X() : i(4) {}
	const int& get() const { return i; }
};

class Y : public X { 
private: 
	const int i;
public: 
	Y() : X(), i(5) {} 
	const int& get() const { return i; }
};

int main() {
	X* x = new X;
	X* y = new Y;
	cout << (y->get()-x->get()) << endl; 
	delete x;
	delete y;

	return 0;
}

