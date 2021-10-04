#include <stdio.h>
#include <string.h>
#include "learn_c.h"

int main(int argc, char *argv[])
{

	if(argc <= 1)
	{
		printf("Error in arguments. Program exits !\n");
		return FAILURE;
	}
	
	if(!reverse_string(argv[1]))
	{
		printf("Error in reverse string\n");

	}
	return SUCCESS;
}
bool reverse_string(char *pString)
{
	char *pStr_start = pString;
	char *pStr_end;
	unsigned usInput_str_len;

	/* Check for validity of the string pointer */
	if(pString == NULL)
	{
		printf("String pointer is NULL\n");
		return FALSE;
	}

	usInput_str_len = strlen(pString);
	printf("Input string of length %u is \"%s\"\n",usInput_str_len,pStr_start);

	if(!(usInput_str_len >> 1))
	{
		/* If string length is 0 or 1, reversed string same as the original string. No reversal needed */
		printf("Reversed string is same as original string.\n");
	}
	else
	{
		pStr_end = pStr_start + usInput_str_len -1;

		for(;pStr_end>pStr_start;--pStr_end,++pStr_start)
		{
			*pStr_start ^= *pStr_end;
			*pStr_end ^= *pStr_start;
			*pStr_start ^= *pStr_end;
		}
		printf("Reversed string is \"%s\"\n", pString);
	}
	return TRUE;
}
