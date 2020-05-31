#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int UINT;

void DisplayCombination(UINT numOfSteps, UINT num);


/* Probem statement: 
 * You have a ladder n-steps in height.  You can either take one step
 * or two steps up the ladder at a time.  How can you find out all the
 * different combinations up the ladder?  Then figure out an algorithm that
 * will actually print out all the different ways up the ladder
/* Approach:
 * For any N, and binary value 0/1, we can have 2 power N possible combinations
 * So, generate all those possible combinations and count the step count
 * if all the step count adds up to N - then its a valid combination for our problem
 */

void StepProblem(UINT numOfSteps) {
	UINT bitCount = 0;
	UINT stepCount = 0;
	UINT count = 0;
	UINT possibleCombinations = pow(2, numOfSteps);
	/* For each such possible ways, see whether its a valid combination */
	for(;count<possibleCombinations; count++) {
		/* this can be done by counting:
		 * no_of_bits which has 0 ==> taking one step at a time
		 * no_of_bits which has 1 ==> taking 2 step at a time
		 * if its equals N, then its a valid way.
		 * so display the sequence accordingly [by printing "1" for bits with 0 and "2" for bits with 1
		 */
		DisplayCombination(numOfSteps, count);
	}
}

void DisplayCombination(UINT numOfSteps, UINT num) {
    UINT count = 0;
	UINT lsb = 0;
	UINT totalSteps = 0;
	UINT index = 0;
	char* combination = malloc(sizeof(char) * (numOfSteps + 1));

	for(;index < numOfSteps; index++) {
		/* Get the LSB */
		lsb = num & 0x01;
		if(lsb == 0x01) {
			/* taking 2 steps at a time */
			totalSteps += 2;
			combination[index] = '2';
		}
		else {
			/* taking 1 step at a time */
			totalSteps += 1;
			combination[index] = '1';
		}
		if(totalSteps == numOfSteps) {
			/* We are done with one possible combination, so display it and break*/
			combination[index+1] = '\0';
			printf("%s\n", combination);
			break;
		}
		/* shift the number a bit so that we get the next bit */
		num = num >> 0x01;
    }
	free(combination);
}
