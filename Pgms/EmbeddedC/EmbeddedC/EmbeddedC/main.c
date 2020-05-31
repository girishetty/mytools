#include <stdio.h>

typedef unsigned char BOOL;

//void Number();
//#include "Header.h"

int main() {
	char string[] = "abcd";
	int count = 0;
	char* str = "Girish Sooral Shetty";
	char format[10];
	BOOL var[10] = { 0 };
#if 0
	unsigned int val = 0;
	char string[] = "Girish";
	char*  pString = NULL;
	Number();
	ArrayTest();
	TestPassStruct();
	TestArrayCallByValue();

	val = OctalToDecimal("12");
	printf("\n12 is %d\n", val);

	val = OctalToDecimal("34562");
	printf("34562 is %d\n", val);

	val = OctalToDecimal("45821");
	printf("45821 is %d\n", val);

	val = OctalToDecimal(NULL);
	printf("NULL is %d\n", val);

	val = OctalToDecimal("776554");
	printf("776554 is %d\n", val);

	val = OctalToDecimal("7777777776554");
	printf("7777777776554 is %u\n", val);

	val = OctalToDecimal("00000000000006554");
	printf("0000000000006554 is %u\n", val);

	pString = ReverseString(string, 6);
	printf("String after reversing : %s\n", pString);
#endif
	int val = 6;
	printf("%.6s\n", str);
	sprintf(format, "%s%d%s", "%.", val, "s\n");
	printf("%.%ds\n", val, str);
	printf(format, str);
	printf(format, str+7);
	printf(format, str+14);
	ReverseString("Welcome to the world of matrix");
	//printf("sqrt of 125348 is %f\n", square_root(125348));
	//printf("sqrt of 334084 is %f\n", square_root(334084));
	//StepProblem(5);
	//count = Permutation(string, 0, 4);
	//printf("no of permutation is : %d\n", count);

	return 0;
}
