#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "amicablenumber.h"

struct amicable* getAmicablePairs(int startnum, int endnum) {
	int i = 0;
	int h, t, s;
	int val1, val2;
	struct amicable* nums = NULL;
	ArrayPair* pairs = NULL;
	
	/* Check for all valid cases */
	if(startnum >= 1 && endnum < MAX_RANGE && startnum < endnum) {
		nums = (struct amicable*) malloc (sizeof(struct amicable));
		/* Allocate integer array of [MAX_AMICABLE_NO][COLUMNS] */
		pairs = (ArrayPair*) malloc(sizeof(int) * MAX_AMICABLE_NO * COLUMNS);
		nums->size = 0;

		for(i = 2; i<= endnum;i++) {
			h = 3 * (int) pow(2, i) - 1;
			t = 3 * (int) pow(2, i-1) - 1;
			s = 9 * (int) pow(2, 2*i-1) - 1;

			/* If all the 3 numbers are prime, then */
			if(isPrime(h) && isPrime(t) && isPrime(s)) {
				val1 = (int) pow (2, i) * h * t;
				val2 = (int) pow (2, i) * s;
				/* check for the valid range */
				if(val1 >= startnum && val1 <= endnum && val2 <= endnum) {
					pairs[nums->size][0] = val1;
					pairs[nums->size][1] = val2;
					nums->size++;
				}
				else if (val1 > endnum || val2 > endnum) {
					/* out of our range, so exit the loop by setting the end condition */
					i = endnum;
				}

			}
		}
		/* if no Amicable number pair found, then */
		if(nums->size == 0) {
			freeAmicable(nums);
			nums = NULL;
		}
		else {
			nums->amicablePair = (int**) pairs;
		}
	}

	return nums;
}


void freeAmicable(struct amicable* p) {
	free(p->amicablePair);
	free(p);
}

bool isPrime(unsigned int num) {
	bool prime = TRUE;
	unsigned int divisor = 3;
	/* First Non-prime no is 4, so continue only if no is > 3 */
	if(num > 3) {
		/* Except for 2, all other even nos are not PRIME */
		if(!IS_ODD(num)) {
			prime = FALSE;
		}
		else  {
			/* All the numbers that are divisible by a number greater than 7 are divisible by a number less 8 */
			/* Which us leaves with, the only numbers left to divide against are 3, 5 and 7 */
			divisor = 3;
			for(; divisor <= 7; divisor += 2) {
				/* 3, 5, 7 are prime numbers */
				if(num == divisor) {
					break;
				}
				/* if its divisible, then its not a PRIME */
				else if(num % divisor == 0) {
					prime = FALSE;
					break;
				}
			}
		}
	}

	return prime;
}

void printAndFreeAmicable(struct amicable* nos) {
	ArrayPair* pairs = NULL;
	int i = 0;

	printf("getAmicablePairs() returns ");
	if(nos) {
		pairs = (ArrayPair*) nos->amicablePair;
		printf("{");
		for(i=0; i<nos->size;i++) {
			printf("{%d, %d} ", pairs[i][0], pairs[i][1]);
		}
		printf("}\n");

		freeAmicable(nos);
	}
	else {
		printf("NULL\n");
	}
}

/* test code */
int main() {
	struct amicable* nos = NULL;
	/* test case# 1 */
	int startnum = 10;
	int endnum = 10000;

	nos = getAmicablePairs(startnum , endnum );
	printAndFreeAmicable(nos);

	/* test case# 2 */
	startnum = 100;
	endnum = 2000;

	nos = getAmicablePairs(startnum , endnum );
	printAndFreeAmicable(nos);

	/* test case# 3 */
	startnum = 100;
	endnum = 10;

	nos = getAmicablePairs(startnum , endnum );
	printAndFreeAmicable(nos);

	return 0;
}
