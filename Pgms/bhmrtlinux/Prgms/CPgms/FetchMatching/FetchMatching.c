#include <stdio.h>

int GetCountOfMatchingNum(int* arrayOne, int* arrayTwo, int sizeOne, int sizeTwo, int findNum) {
	int indexOne = 0;
	int indexTwo = sizeTwo-1;
	int count = 0;

	if(findNum < (arrayOne[0] + arrayTwo[0]) || findNum > (arrayOne[sizeOne-1] + arrayTwo[indexTwo])) {
		printf("%d does not fall under the range\n", findNum);
		return -1;
	}
	
	for(;indexOne < sizeOne && indexTwo >= 0;) {
		if(findNum < (arrayOne[indexOne] + arrayTwo[indexTwo])) {
			/* decrease 2nd index */
			indexTwo--;
		}
		else if(findNum > (arrayOne[indexOne] + arrayTwo[indexTwo])) {
			/* increase 1st index */
			indexOne++;
		}
		else { 
			/* we have got one, increase the count */
			printf("%d + %d = %d\n", arrayOne[indexOne], arrayTwo[indexTwo], findNum);
			count++;
			indexTwo--;
			indexOne++;
		}
	}

	return count;
}

void GetSortedList(int* array, int count) {
	int index = 0;
	printf("Enter %d elements in sorted (ascending) order\n", count);
	for(;index < count; index++) {
		scanf("%d", &array[index]);
	}

	return;
}

int main() {
	int arrayOne[10];
	int arrayTwo[10];
	int index;
	int findNum;
	int count = 0;

	GetSortedList(arrayOne, 10);
	GetSortedList(arrayTwo, 10);

	printf("Enter a num to be checked : ");
	scanf("%d", &findNum);

	count = GetCountOfMatchingNum(arrayOne, arrayTwo, 10, 10, findNum);
	printf("Found %d occurance of %d\n", count, findNum);

	return 0;
}

