#include <stdio.h>
#include <sys/param.h>

#define MYMACRO 20
#if defined(MYMACRO) && MYMACRO == 1 
int glob;
#endif

#define TRUE 0

int main() {
	int n;
	int val = TRUE;
	int (*p)[5]; /* Pointer to Array */
	int *q[5]; /* Array of Pointers */
	int array[5];
	int a, b;
	p = array;
	/*q = array;*/
	q[0] = &a;
	q[1] = &b;
	p++;
	printf("value is : %d", p);
	printf("Enter a No:");
	scanf("%d", &n);
	fun();
	return 0;
}

int fun() {
	printf("Hello\n");
	return 0;
}

