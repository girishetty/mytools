# include <iostream>
using namespace std;
# include "Header.h"

unsigned int Byte::BitsCount (Bit bit) const{
    unsigned int count = 0;
    if (bit0 == bit) {
        count ++;
    }
    if (bit1 == bit) {
        count ++;
    }
    if (bit2 == bit) {
        count ++;
    }
    if (bit3 == bit) {
        count ++;
    }
    if (bit4 == bit) {
        count ++;
    }
    if (bit5 == bit) {
        count ++;
    }
    if (bit6 == bit) {
        count ++;
    }
    if (bit7 == bit) {
        count ++;
    }

    return count;
}

// Test codes..
void TestChar () {
    DataType<char> charData('A');
    charData.Info();
}

void TestShort () {
    DataType<short> shortData(0x1234);
    shortData.Info();
}

void TestInteger () {
    DataType<int> intData(0xABCD1234);
    intData.Info();
}

int main () {
    TestChar();
    TestShort();
    TestInteger();
    return 0;
}
