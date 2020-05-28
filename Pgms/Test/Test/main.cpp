#include <iostream>
#include <stdio.h>

using namespace std;

void foo() {
	int x = 100;
	int array[10];
	int* p = array + 3;
	printf("%d\n %d\n", --x? ++x : --x, sizeof(array)/sizeof(*array));
	printf("%p %p\n", array, p);
	if(-5)
		printf("\n%d\n", 64/4*4);
	if(!!x)
		printf("purrr: %d\n", !x);
}

namespace {

	int my;
}

void GetInfo(char** info) {
	char* p = "Girish";
	//*info = p;
	*info = "Girish";
}

struct A {
	char a;
	int b;
	char c;
	long long d;
};

struct B {
	int b;
	char a;
	char c;
};

struct C {
	char a;
	char c;
	int b;
};

struct D {
	char a;
    char c;
};

struct E {
	char a;
};

int main() {
	cout<<"Hello"<<endl;
	int m = 20.23;
	my = 20;

	printf("sizeof(Girish) : %d\n", sizeof("Girish"));
	char* info = NULL;
	GetInfo(&info);
	printf("Info: %s\n", info);
	info = "Shetty";
	printf("Info: %s\n", info);

	printf("Sizeof(A): %d  Sizeof(B): %d  Sizeof(C): %d  Sizeof(D): %d  Sizeof(E): %d\n", 
		sizeof(A), sizeof(B), sizeof(C), sizeof(D), sizeof(E));

	A a;
	printf("Address of A::a : %p\n", &a.a);
	printf("Address of A::b : %p\n", &a.b);
	printf("Address of A::c : %p\n", &a.c);
	printf("Address of A::d : %p\n", &a.d);

	return 0;
}
