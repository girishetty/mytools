#include <stdio.h>

int permCount = 0;
void Interchange(char* array, int srcIndex, int destIndex) {
	char temp = array[srcIndex];
	array[srcIndex] = array[destIndex];
	array[destIndex] = temp;
}

int Permutation(char* array, int currentIndex, int size) {
	int index = currentIndex;
	if(currentIndex == size) {
		printf("%s\n", array);
		permCount++;
		return permCount;
	}
	for(;index < size; index++) {
		Interchange(array, currentIndex, index);
		Permutation(array, currentIndex+1, size);
		Interchange(array, index, currentIndex);
	}
	return permCount;
}

