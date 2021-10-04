# include <iostream>
using namespace std;
# include "Header.h"

unsigned int Byte::BitsCount (Bit bit) const{
	unsigned int count = 0;
	if(bit0 == bit)
		count ++;
	if(bit1 == bit)
		count ++;
	if(bit2 == bit)
		count ++;
	if(bit3 == bit)
		count ++;
	if(bit4 == bit)
		count ++;
	if(bit5 == bit)
		count ++;
	if(bit6 == bit)
		count ++;
	if(bit7 == bit)
		count ++;

	return count;
}

// Test codes..

void TestChar () {
	DataType<char> charData(123);
	cout<<"MSB is : "<<int(charData.GetMSB())<<endl;
	cout<<"LSB is : "<<int(charData.GetLSB())<<endl;
	cout<<"Total Number of Bits those are One  are : "<<charData.BitsCount(1)<<endl;
	cout<<"Total Number of Bits those are Zero are : "<<charData.BitsCount(0)<<endl;
}

void TestShort () {
	DataType<short> shortData(123);
	cout<<"MSB is : "<<int(shortData.GetMSB())<<endl;
	cout<<"LSB is : "<<int(shortData.GetLSB())<<endl;
	cout<<"Total Number of Bits those are One  are : "<<shortData.BitsCount(1)<<endl;
	cout<<"Total Number of Bits those are Zero are : "<<shortData.BitsCount(0)<<endl;
}

void TestInteger () {
	DataType<int> intData  = 123;
	cout<<"MSB is : "<<int(intData.GetMSB())<<endl;
	cout<<"LSB is : "<<int(intData.GetLSB())<<endl;
	cout<<"Total Number of Bits those are One  are : "<<intData.BitsCount(1)<<endl;
	cout<<"Total Number of Bits those are Zero are : "<<intData.BitsCount(0)<<endl;
}


int main () {

	TestChar();
	TestShort();
	TestInteger();
	return 0;
}
