#include <stdio.h>
//Different ways of using __aligned__ attribute
struct __attribute__((__aligned__(8))) A {
	int intMem;
	char charMem;
	int iMem;
};

struct B {
	int intMem;
	char charMem;
	int iMem;
} __attribute__((__aligned__(1))) ;

struct __attribute__((__aligned__(2))) C {
	int intMem;
	char charMem;
	int iMem;
};

struct D {
	int intMem;
	char charMem;
	int iMem;
} __attribute__((__aligned__(4)));

struct __attribute__((__packed__)) A1 {
	int intMem;
	char charMem;
	int iMem;
};

struct B1 {
	int intMem;
	char charMem;
	int iMem;
} __attribute__((__packed__)) ;

struct __attribute__((__packed__)) C1 {
	int intMem;
	char charMem;
	int iMem;
};

struct D1 {
	int intMem;
	char charMem;
	int iMem;
} __attribute__((__packed__));

/* 
One can specify granuality here.
If you dont use pragma pack, default is 4 byte
*/
#pragma pack(1)
struct AA {
	int intMem;
	char charMem;
	int iMem;
};

int main() {
	printf("Struct and Attribute Aligned...\n");
	printf("Size of Struct A is : %d\n", sizeof(struct A));
	printf("Size of Struct B is : %d\n", sizeof(struct B));
	printf("Size of Struct C is : %d\n", sizeof(struct C));
	printf("Size of Struct D is : %d\n", sizeof(struct D));

	printf("Struct and Attribute Packed...\n");
	printf("Size of Struct A1 is : %d\n", sizeof(struct A1));
	printf("Size of Struct B1 is : %d\n", sizeof(struct B1));
	printf("Size of Struct C1 is : %d\n", sizeof(struct C1));
	printf("Size of Struct D1 is : %d\n", sizeof(struct D1));

	printf("Struct and pragma pack macro...\n");
	printf("Size of Struct AA is : %d\n", sizeof(struct AA));
	return 0;
}

