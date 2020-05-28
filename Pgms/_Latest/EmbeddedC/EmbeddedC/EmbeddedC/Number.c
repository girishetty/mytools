#include <stdio.h>

signed int Negate(signed int val);

#define TEN_MILLION 10000000

void Number() {
	double val1 = 0.0;
	double val2 = 0.0;
	unsigned int index = 0;
	signed int  sint = 222;
	unsigned int uint = 222;

	for(;index<TEN_MILLION;index++) {
		val1 += 0.10;
	}

	val2 = 0.10 * TEN_MILLION;
	printf("Value with rep Addition : %f\n", val1);
	printf("Value with Multiplication : %f\n", val2);

	printf("Negate of %d is %d\n", -289, Negate(-289));
	printf("Negate of %d is %d\n", 786765, Negate(786765));
	printf("Negate of %d is %d\n", -45321, Negate(-45321));
}

signed int Negate(signed int val) {
	signed int negated = ~val + 01;
	return negated;
}

#if 0
void PassArray(int* a) {
#else
void PassArray(int a[]) {
//void PassArray(int a[10]) {
#endif
int i = 0;
	for(;i<10;i++) {
#if 0
		a[i] += 20;
		printf("%d  ", a[i]);
#else
		*a += 20;
		printf("%d  ", *a);
		a++;
#endif
	}
	printf("\n");
}

void ArrayTest() {
	int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int i = 0;
	int* p1 = 0;
	int* p2[10] = {0};
	int (*p3)[10] = 0;
	PassArray(array);
	for(;i<10;i++) {
		printf("%d  ", array[i]);
	}

	printf("\nBefore: p1 = %d p2 = %d p3 = %d\n", p1, *p2, p3);
	p1++;
	//p2++; //its error - same array++;
	p3++;
	printf("After: p1 = %d p2 = %d p3 = %d\n", p1, *p2, p3);
};


typedef struct {
	char name[10];
	int age;
}Identity;

typedef struct {
	int array[10];
}Array;

void PassStruct(Identity id) {
	id.age = 25;
	printf("Name : %s   Age : %d\n", id.name, id.age);
}

void TestPassStruct() {
	Identity id = {"Girish", 30 };
	Identity id2 = {"xyz", 20 };
	PassStruct(id);
	printf("Name : %s   Age : %d\n", id.name, id.age);
	id = id2;
	printf("Name : %s   Age : %d\n", id.name, id.age);
}

void ArrayCallByValue(Array a) {
	int i = 0;
	for(;i<10;i++) {
		a.array[i] += 10;
		printf("%d  ", a.array[i]);
	}
	printf("\n");
}

void TestArrayCallByValue() {
	Array a;
	int i = 0;
	for(;i<10;i++) {
		a.array[i] = i + 1;
		printf("%d  ", a.array[i]);
	}
	printf("\n");
	ArrayCallByValue(a);
	for(i=0;i<10;i++) {
		printf("%d  ", a.array[i]);
	}
}
