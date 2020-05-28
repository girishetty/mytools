#include <stdio.h>


void PtrTestOne () {
int *pint1;
int (*pint4)[4];
pint1 = 0;
pint4 = 0;

printf("Value of pint1 is : %d\n", pint1);
printf("Value of pint4 is : %d\n", pint4);

pint1++;
pint4++;

printf("Value of pint1 is : %d\n", pint1);
}

void PtrTestTwo () {
int a[ ][4] = {{1,2,3,4},{5,6,7,8}};
printf("Value is %d\n", *(a[1]+2));
}

void TestArray(){
	int a[5], i;
	for (i=0; i<5; ++i)
		a[(i+3) %5] = 2*i;
	for (i=0; i<5; ++i)
		printf("%d " , a[i]);
}

void RecursiveFun(int A[ ], int size)
{
	if (size > 0) {
		A[0] = 0;
		RecursiveFun(A+1, size-1);
	}
}

void TestArrayTwo()
{
	int array[10];
	int i;
	printf("\nUninitialized Values are: ");
	for(i=0; i<10; i++)
	printf("%d  ", array[i]);
	printf("\nNew Values are: ");
	RecursiveFun(array, 10);
	for(i=0; i<10; i++)
	printf("%d  ", array[i]);
}
int main()
{
//PtrTestOne();
//PtrTestTwo();
//TestArray();
TestArrayTwo();
return 0;
}
