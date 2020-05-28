#include <stdio.h>

const int intConst = 20;
char* ptr= "Hello";
char global= 'C';
char global1;

void Foo(char* p) {
	printf("Inside Foo: Address of p is : %x\n", p);
}

void Fooo(char* p, char* p1) {
	printf("Inside Foo: Address of p is : %x\n", p);
}

int main() {
	char* p = "Hello";
	char* mem = 0;

	mem = (char*) malloc(10);

	printf("Address of Foo is : %p\n", &Foo);
	printf("Address of main is : %p\n", &main);
	printf("Address of intConst is : %p\n", &intConst);
	printf("Address of ptr is : %p\n", &ptr);
	printf("ptr Points to : %p\n", ptr);
	printf("Address of p is : %p\n", &p);
	printf("p Points to : %p\n", p);
	printf("Address of mem is : %p\n", &mem);
	printf("mem Points to : %p\n", mem);
	printf("Address of global is : %p\n", &global);
	printf("Address of global1 is : %p\n", &global1);

	Foo(p);
	Foo("Hello");
	Fooo("Hello","\n");

	free(mem);
	return 0;
}


