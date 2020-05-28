			#define C_OR_CPP 1 //  Change value to 1 here if you are submitting a C solution for this problem.
								//  Change value to 2 here if you are submitting a C++ solution for this problem.

struct amicable			// Dont change here
{
	int **amicablePair;
	int size;
};

void dsmain();

#if C_OR_CPP == 1  // Dont change here

	struct amicable *getAmicablePairs(int startnum, int endnum);

#endif



#if C_OR_CPP == 2  // Dont change here

class AmicableNumber
{
	public:
		struct amicable *getAmicablePairs(int startnum, int endnum);
};
#endif

/* other declaration/constants used by the algorithm code */

/* Max 100 Amicable numbers */
#define MAX_AMICABLE_NO		1000
#define MAX_RANGE			15000
#define COLUMNS				2

#define TRUE				1
#define FALSE				0

#define IS_ODD(number)		((number) & 0x1)

typedef int ArrayPair[COLUMNS];
typedef unsigned int bool;


/* function to free amicable structure */
void freeAmicable(struct amicable* p);

/* function to get some of all divisor of a number*/
int sumOfAllDivisiors(int num);

/* function to check a prime no */
bool isPrime(unsigned int num);

/* function to print and free amicable no */
void printAndFreeAmicable(struct amicable* nos);
