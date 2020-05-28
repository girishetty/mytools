#include <stdio.h>

typedef union {
	unsigned int val;
	unsigned char data[4];
}BYTE;

void Swap(unsigned char* ch1, unsigned char* ch2) {
	unsigned char ch;
	ch = *ch1;
	*ch1 = *ch2;
	*ch2 = ch;
}

unsigned int ChangeByteOrder(unsigned int num) {
	int ret;
	int i = 0;
	BYTE byte;
	byte.val = num;

	Swap(&byte.data[0], &byte.data[3]);
	Swap(&byte.data[1], &byte.data[2]);
	return byte.val;
}

int main(){
	BYTE byte;
	int num;
	printf("Enter a Number : ");
	scanf("%d", &num);
	printf("After Changing to Big Endian %x is %x : \n", num, ChangeByteOrder(num));

	return 0;
}
