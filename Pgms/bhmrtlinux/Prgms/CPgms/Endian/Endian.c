#include <stdio.h>

typedef union {
	unsigned short val;
	char BYTES[2];

}TWOBYTES;

int main() {
	TWOBYTES twobytes;
	twobytes.val = 0x0001;

	if(twobytes.BYTES[0] == 0x01) {
		printf("BIG ENDIAN SYSTEM\n");
	}
	else {
		printf("LITTLE ENDIAN SYSTEM\n");
	}
	return 0;
}

