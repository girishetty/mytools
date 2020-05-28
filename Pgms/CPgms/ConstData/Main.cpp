#if 0
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
	const int a = 20;
	int* ptr1  = (int*) &intConst;
	//int array[a];
	char* p = "Hello";
	char* mem = 0;
	printf("ptr has  %s\n", ptr);

	ptr = &global;
	*ptr = 'h';
	printf("ptr has  %s\n", ptr);

	//mem = malloc(10);

	//printf("a is %d *ptr1 is %d\n", intConst, *ptr1);
	//*ptr1 = 40;
	//printf("a is %d *ptr1 is %d\n", intConst, *ptr1);

	printf("Address of Foo is : %x\n", &Foo);
	printf("Address of main is : %x\n", &main);
	printf("Address of intConst is : %x\n", &intConst);
	printf("Address of ptr is : %x\n", &ptr);
	printf("ptr Points to : %x\n", ptr);
	printf("Address of p is : %x\n", &p);
	printf("p Points to : %x\n", p);
	printf("Address of mem is : %x\n", &mem);
	printf("mem Points to : %x\n", mem);
	printf("Address of global is : %x\n", &global);
	printf("Address of global1 is : %x\n", &global1);

	Foo(p);
	Foo("Hello");
	Fooo("Hello","\n");

	//free(mem);
	return 0;
}

#endif


#include <iostream>
using namespace std;


const int* const p1 = 0;

int main() {
	int a = 20;
	int* p = (int*)&p1;
	cout<<"p1 is : "<<p1<<"address of p1 is : "<<&p1<<endl;
	p = (int*)0x3456;
	cout<<"p1 is : "<<p1<<"address of p1 is : "<<&p1<<endl;

	return 0;
}
