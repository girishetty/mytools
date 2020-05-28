#include <stdio.h>
#include <string.h>

typedef int int_array[10];

int main() {
	int_array array;
	int i = 0;
	memset(array,0, sizeof(array));
	array[0] = 10;
	array[2] = 20;
	for(; i<10; i++)
		printf("array[%d] is %d \n", i, array[i]);
	return 0;
	}

