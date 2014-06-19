# include <iostream>
using namespace std;

typedef unsigned char Bit;

struct Byte {
private:
	Bit bit0 : 1;
	Bit bit1 : 1;
	Bit bit2 : 1;
	Bit bit3 : 1;
	Bit bit4 : 1;
	Bit bit5 : 1;
	Bit bit6 : 1;
	Bit bit7 : 1;

public:
	Bit GetMSB () const {
		return bit7;
	}

	Bit GetLSB () const {
		return bit0;
	}
	unsigned int BitsCount (Bit bit) const;
};

template <class Type> union  DataType {

	enum {
		EMaxSize = 10
	};
private:

	Type data;
	Byte byte[EMaxSize];

public:
	DataType (Type d = 0) : data (d) { }
	DataType (DataType& d) : data (d.data) { }

	void Set(Type d) { 
		data = d;
	}

	void Set(DataType& d) { 
		data = d.data;
	}

	Bit GetMSB () const {
		return byte[sizeof(Type)-1].GetMSB();
	}

	Bit GetLSB () const {
		return byte[0].GetLSB();
	}
	unsigned int BitsCount (Bit bit) const;
};

template <class Type> 
unsigned int DataType<Type>::BitsCount (Bit bit) const {
	unsigned int count = 0;
	unsigned int size = sizeof(Type);

	for(int i=0; i<size; i++)
		count += byte[i].BitsCount(bit);

	return count;
}
