#include <stdio.h>

void CallThisFunction(int a, int b, int c, int d, int e) {
        printf("a is : %d\n", a);
        printf("b is : %d\n", b);
        printf("c is : %d\n", c);
        printf("d is : %d\n", d);
        printf("e is : %d\n", e);

        return;
}

void UnaryOperators () {
	int i, j;
	j = (i = 4, i++ + i++);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i = 4, i++ + ++i);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i = 4, ++i + i++);
	printf("i is : %d and j is : %d\n", i, j);
	j = (i = 4, ++i + ++i);
	printf("i is : %d and j is : %d\n", i, j);
}

int main () {
        int i;

        CallThisFunction(i=5, i++, ++i, i+4, i=0);
	UnaryOperators ();
        return 0;
}

