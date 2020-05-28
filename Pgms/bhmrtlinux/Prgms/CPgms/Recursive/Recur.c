#include <stdio.h>

struct X{
	int a;
	int b;
};

int Foo() {
	struct X array[2] = { {10, 12}, { 14, 16}};
	struct X* p;
	p = &array[0];
	p++;
	printf("Value is : %d\n", *p);
}

void Fill_Ary(int A[ ], int N)
{
	if (N != 0) {
		--N;
		A[N] = N;
		Fill_Ary(A,N);
	}
}

int main() {
	int array[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int i;
	Fill_Ary(array, 10);
	for(i=0; i<10; i++)
	printf("  %d  ", array[i]);
	Foo();

	return 0;
}
