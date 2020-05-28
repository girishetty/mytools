#include <stdio.h>

int fun(int aval ){
	return aval;
}

int main() {
	int i = !fun(0);
	int j = !!fun(0);
	printf("i is %d\n", i);
	printf("j is %d\n", j);
	return 0;
}
