#include <stdio.h>

int globVar = 10;
int globVar;// = 20;
int globVar;
//char globVar;

void OpPrecedanceTest(){
	int a = 20, b = 10;
	int c = 0;

	if( a < b && a++)
		c = 40;

	printf("a is %d b is %d c is %d\n", a, b, c);
}

void IncrementOpTest(){
	int i, j;
	j = (i=7, i++ + i++);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i=7, i++ + ++i);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i=7, ++i + i++);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i=7, ++i + ++i);
	printf("i is : %d and j is : %d\n", i, j);
}

void CallThisFunction(int a, int b, int c, int d, int e) {
	printf("a is : %d\n", a);
	printf("b is : %d\n", b);
	printf("c is : %d\n", c);
	printf("d is : %d\n", d);
	printf("e is : %d\n", e);

	return; 
}

int main () {
	int i=1;

	CallThisFunction(i=5, i++, ++i, i+4, i=0);
	printf("i is now : %d\n", i);

	IncrementOpTest();
	OpPrecedanceTest();
	return 0;
}

