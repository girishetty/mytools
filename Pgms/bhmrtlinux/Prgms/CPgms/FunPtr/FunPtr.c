#include <stdio.h>
#include <stdlib.h>

typedef void (*FunPtr) (int, int);

void Foo(int a, int b) {
	printf("Inside Foo with %d and %d\n", a, b);
}

int main() {
	FunPtr ptr = Foo;
	printf("Inside Main\n");
	ptr(10, 20);
	(*ptr)(20, 30); 
	/* free(ptr); //CRASHES */

	printf("Returning from Main\n");
	return 0;
}

