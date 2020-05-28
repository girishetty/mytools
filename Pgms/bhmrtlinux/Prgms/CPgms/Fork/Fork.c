#include <stdio.h>

int main () {
	int count = 1;

	fork();
	fork();
	fork();

	printf("Count is %d\n", ++count);
	/*
	printf("Current Process Id is : %d\n", getpid());
	printf("Parent  Process Id is : %d\n", getppid()); */
	return 0;
}

