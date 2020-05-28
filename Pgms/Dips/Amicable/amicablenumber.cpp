#include "Amicable.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct amicable* getAmicablePairs(int startnum, int endnum) {
	int i = 0;
	int h, t, s;
	int val1, val2;
	struct amicable* nums = (struct amicable*) malloc (sizeof(struct amicable));
	/* Allocate integer array of [MAX_AMICABLE_NO][COLUMNS] */
	nums->numbers = (ArrayPair*) malloc(sizeof(int) * MAX_AMICABLE_NO * COLUMNS);
	nums->size = 0;

	for(i = startnum; i<= endnum;i++) {
		h = 3 * pow(2, i) - 1;
		t = 3 * pow(2, i-1) - 1;
		s = 9 * pow(2, 2*i-1) - 1;

		/* If all the 3 numbers are prime, then */
		if(isPrime(h) && isPrime(t) && isPrime(s)) {
			val1 = pow (2, i) * h * t;
			val2 = pow (2, i) * s;
			if(val1 < endnum && val2 < endnum) {
				nums->numbers[nums->size][0] = val1;
				nums->numbers[nums->size][1] = val2;
				nums->size++;
			}
			else {
				/* out of our range, so exit the loop by setting the end condition */
				i = endnum;
			}

		}
	}

	return nums;
}


void free_amicable(struct amicable* p) {
	free(p->numbers);
	free(p);
}

bool isPrime(unsigned int num) {
	bool prime = true;
	/* 4 is the first non prime number. */
    if(num > 3)
    {
        /* The only even prime number is 2. */
        if(!IS_ODD(num))
        {
            prime = false;
        }
        else
        {
            /* The only numbers left to divide against are 3, 5 and 7. */
            /* All numbers that are divisible by a number > 7 are divisible */
            /* by a number < 8. */
            unsigned int divisor = 3;
            for(; divisor <= 7; divisor += 2)
            {
                /* 3, 5, 7 are prime numbers. */
                if(num == divisor)
                {
                    break;
                }
                else if(num % divisor == NO_REMAINDER)
                {
                    prime = false;
					break;
                }
            }
        }
    }

    return prime;
}

