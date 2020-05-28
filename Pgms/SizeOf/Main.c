#include <stdio.h>

int main() {
	unsigned int a;
	int i = sizeof(-1);
	printf("i is %d\n", i);
	a = sizeof(-1) + -10;
	printf("\na is %u\n", a);

	if(sizeof(-1) < -1) {
		printf("Hi\n");
	}
	else {
		printf("Hello\n");
	}
	return 0;
}