#include <stdio.h>

typedef unsigned int UINT;

UINT GetBitWithOneCount(UINT aNum) {
	UINT count = 0;
	for(;aNum;count++) {
		aNum = aNum & (aNum-1);
	}
	return count;
}

int main() {
	int num;
	printf("Enter a Number : ");
	scanf("%d", &num);

	printf("Number of Bits with 1 in %d is %d\n", num, GetBitWithOneCount(num));

	return 0;
}

