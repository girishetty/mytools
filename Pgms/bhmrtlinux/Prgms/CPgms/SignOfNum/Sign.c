#include <stdio.h>

# define CHAR_BIT  8  /* the number of bits per byte (normally 8). */
int main() {
	int num;      /* we want to find the sign of v */
	int sign;   /* the result goes here  */
#if 0
	printf("Enter Num : ");
	scanf("%d", &num);

	sign = -(num < 0);  // if v < 0 then -1, else 0. 
	printf("Sign of %d is %d\n", num, sign);

	sign = num >> (sizeof(int) * CHAR_BIT - 1); 
	printf("Sign of %d is %d\n", num, sign);
	/* And the portable code is: */
#endif
	sign = -(int)((unsigned int)((int)num) >> (sizeof(int) * CHAR_BIT - 1));
#if 0
	printf("Sign of %d is %d\n", num, sign);

#endif
	return 0;
}

