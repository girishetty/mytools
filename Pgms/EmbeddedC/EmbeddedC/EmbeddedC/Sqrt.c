#include <math.h>

typedef unsigned int UINT;

UINT GetNumDigits(UINT number) {
	UINT no_of_digits = 0;
	while(number!=0) {
		number /= 10;
		no_of_digits++;
	}
	return no_of_digits;
}

/*
 * Problem:
 * Write an algorithm to calculate the square root of a number.
 * solution:
 * Implementing Babylonian method algorithm for approximation
*/

double square_root(UINT number) {
	double approx_sqrt = 0.0;
	double next_approx_sqrt = 0.0;
	double approximation = 0.0;
	UINT power_factor = 0;
	UINT digits = GetNumDigits(number);

	/* check whether no_of_digits is odd or even and set up the things accordingly */
	if(digits & 0x01) {
		power_factor = (digits - 1) /2;
		approx_sqrt = 2.0;
	}
	else {
		power_factor = (digits - 2) /2;
		approx_sqrt = 6.0;
	}

	/* get the initial approximation using Babylonian method*/
	next_approx_sqrt = approx_sqrt * pow(10, power_factor);

	do {
		approx_sqrt = next_approx_sqrt;
		/* get the next approximation using Babylonian method*/
		next_approx_sqrt = 0.5 * (approx_sqrt + (number/approx_sqrt));
		approximation = abs(approx_sqrt - next_approx_sqrt);
		/* try for 5-6 decimal digit approximation */
	}while( approximation > 0.0001);

	return next_approx_sqrt;
}
