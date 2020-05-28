#include <stdio.h>

int main() {
	int a=10;
	int b=20;
	int c;

	if(a>20 && ++a)
		c=40;
	printf("%d %d %d\n",a,b,c);

	return 0;
}
