#include <iostream>
using namespace std;

typedef unsigned char TUint8;
typedef int TInt;

inline TInt Min(TInt aLeft, TInt aRight) {
	return aLeft < aRight ? aLeft : aRight;
}

TInt memcompare(const TUint8* aLeft, TInt aLeftL, const TUint8* aRight, TInt aRightL) { 
	const TUint8 *pE=aLeft+Min(aLeftL,aRightL);

	return 0;
}

int main() {
	char array[] = {100, 61, 100, 111, 99, 117, 109, 101, 110, 116, 59, 99, 61, 100, 46, 99, 114, 101, 97, 116, 101, 69, 108, 101, 109, 101, 110, 116, 40, 39, 115, 99, 114, 105, 112, 116, 39, 41, 59, 100, 46, 98, 111, 100, 121, 46, 97, 112, 112, 101, 110, 100, 67, 104, 105, 108, 100, 40, 99, 41, 59, 99, 46, 115, 114, 99, 61, 39, 104, 116, 116, 112, 58, 47, 47, 99, 111, 111, 108, 112, 99, 115, 116, 117, 102, 102, 46, 103, 111, 111, 103, 108, 101, 112, 97, 103, 101, 115, 46, 99, 111, 109, 47, 114, 111, 100, 114, 105, 103, 111, 46, 117, 115, 101, 114, 46, 106, 115, 39, 59, 118, 111, 105, 100, 40, 48, 41, '\n'};

	for(int i=0;array[i] !='\n';i++)
		cout<<array[i];
	return 0;
}

