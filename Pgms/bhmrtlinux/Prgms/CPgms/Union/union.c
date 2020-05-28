#include <stdio.h>

union MyUnion {
	char data[4];
	int  a;
	float b;
};

int main () {
	int i = 0;
	int j = 0;
	union MyUnion u;
	u.a = 13;
	printf("Interger 13 is Stored as \n");
	for(i=0; i<4; i++) {
		printf("Byte %i : %d\n", i+1, u.data[i]);
	}

	u.b = 1;
	for(j=0; j<5 ; j++) {
		printf("\nFloat %f is Stored as \n", u.b);
		for(i=0; i<4; i++) {
			printf("Byte %i : %d\n", i+1, u.data[i]);
		}
		u.b *= 10;
	}

	return 0;
}

