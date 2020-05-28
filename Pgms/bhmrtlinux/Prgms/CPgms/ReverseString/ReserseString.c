#include <stdio.h>
#include <string.h>

void ReverseMe(char* const ptr, int size) {
	int index = 0;
	for(size = size-1; index < size; index++,size--) {
		/* swap elements in index and size location*/
		ptr[index] ^= ptr[size] ^= ptr[index] ^= ptr[size] ;
	}
}

void ReverseWithTokens(char* const ptr, int size) {
	int index = 0;
	int wordIndex = 0;

	/* reverse whole String */
	ReverseMe(ptr, size);

	/* Get each word and swap them individually */
	for(; index<size;) {
		for(;ptr[wordIndex] != ' ' && ptr[wordIndex] != '\0'; wordIndex++);
			ReverseMe(ptr+index, wordIndex-index);
		wordIndex ++;
		index = wordIndex;
	}
}

int main() {
	char array[] = "Hello Hi How Are You?";
	printf("String After Reversing : %s\n", array);
	ReverseWithTokens(array, strlen(array));
	printf("String After Reversing : %s\n", array);

	return 0;
}


