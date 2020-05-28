# include <stdio.h>

#define CONCAT_STRING(a,b)			CONCAT_STRING_HIDDEN(a, b)
#define CONCAT_STRING_HIDDEN(a,b)	a ## b

void StringCatTest() {
	char* pStr1 = "Girish";
	char* pStr2 = "Shetty";

	printf("\nConcatenated String is : %s\n", CONCAT_STRING("Girish", "Shetty"));

	printf("\nConcatenated String is : %s\n", "CONCAT_STRING(pStr1, Tmp)");
	printf("\nConcatenated String is : %s\n", CONCAT_STRING_HIDDEN("pStr1", "Tmp"));
	printf("\nConcatenated String is : %s\n", CONCAT_STRING("Girish","Shetty"));
	printf("\nConcatenated String is : %s\n", CONCAT_STRING_HIDDEN("Girish","Shetty"));
}

int Function (int a, int b, int c, int d, int e) {
	printf("Inside function now\n");
	printf("%d  %d  %d  %d  %d\n", a, b, c, d, e);
	return a+b+c+d+e;
}


void CallThisFunction(int a, int b, int c, int d, int e) {
        printf("a is : %d\n", a);
        printf("b is : %d\n", b);
        printf("c is : %d\n", c);
        printf("d is : %d\n", d);
        printf("e is : %d\n", e);
}

void ArgPassingTest () {
        int i;

        CallThisFunction(i=5, i++, ++i, i+4, i=0);
		printf("i is now : %d\n", i);
}


void PassingArgTest () {
	int i = 1;
	int j=0;

	printf("%d  %d  %d  %d  %d\n", i++, ++i, i=4, --i, ++i + i --, i);
	printf("now i is : %d\n",i);

	
	printf("Trying function now\n");
	i=1;
	i = Function(i++, ++i, i=4, --i, ++i + i --, i);
	printf("now i is : %d\n",i);

	i = 1;
	j=(i, ++i);
	printf("j is %d\n", j);
}

int a;
int a;
int a;


int main () {

	StringCatTest();
	//PassingArgTest ();
	ArgPassingTest ();
	return 0;
}