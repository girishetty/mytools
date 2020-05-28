#include <stdio.h>

#if 0
char* ReverseString(char* pString, int aLength) {
	if(aLength == 1)
		return pString;
	ReverseString(pString + 1, aLength - 1);
	pString[aLength - 1] = pString[0];
	return pString;
}
#endif

typedef unsigned int UINT;

/*
 * Problem:
 * Given a const null terminated string containing a sentence, can you
 * print out the words of the sentence in reverse without changing any data
 * and without creating a temporary string buffer?  Do not use any helper
 * functions, write it all out on your own.  (ie "The dog is fast"
 * becomes "fast is dog The")
 */
void ReverseString(const char* string) {
	UINT length = 0;
	const char* const ptemp = string;
	int index = 0;
	UINT wordBoundary = 0;
	UINT wordIndex = 0;

	if(string) {
		/* get the length of the string */
		for(;ptemp[length]!='\0';ptemp,length++);
		/* Start reading individual words from the end */
		for(wordBoundary = length, index = length; index >= 0; index--) {
			if(string[index] == ' ' || index == 0) {
				/* we have found a word now */
				if(index == 0) {
					/* to make sure that we dont skip very first characetr */
					index --;
				}
				/*
				 * print the word character by character
				 * personally I would prefer to use something like printf("%.6s\n", string+index);
				 * for printing the word, but as the width/lenght of the word is decided during run time
				 * we would need sprintf:
				 * char fornat[10];
				 * int width = wordBoundary - index;
				 * sprintf(format, "%s%d%s", "%.", width, "s\n");
	             * printf(format, string+index);
				 * but as we are not suppose to use extra memory (problem defn), I am using looping
                 */
				for(wordIndex = index+1; wordIndex <= wordBoundary; wordIndex++) {
					putchar(string[wordIndex]);
				}
				wordBoundary = index;
			}
		}
	}
}
