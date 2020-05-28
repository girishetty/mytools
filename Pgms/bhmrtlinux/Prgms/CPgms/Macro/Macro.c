#include<stdio.h> 

/* Write something in this function (change) so that the output of printf in 
main function should give 5 . Do not change the main function */ 

//Solution 1
void change() { 
	#define int int dummy; 
	#define i 5, dummy 
} 

//Solution 2
/*
void change() { 
	int* ptr = (int *) &ptr; 
	ptr++; 
	ptr++; 
	// now the pointer points to the stored return address of the function
	*ptr += 4; // that 4 should be the instruction length 
	//now the return address will be pointing to the printf statement.. 
	//and hence we skip the statement i=10; 
	return;
} */

int fact() {
	return 1;
}

#define fact(x) (x) * getfact(x-1)

#define getfact(x) (x) * fact(x-1)

int main() { 
	int i=5; 
	change(); 
	i=10; 
	printf("i is %d!!!\n",i); 
	fact(5);
	printf("Fact of 5 is : %d", fact(5));
	return 0;
} 

