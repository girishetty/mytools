/* 
Function to get the length of the string 
Input: unsigned char string as pointer
Output: length
Asssumes that the sting is valid one
*/
unsigned int GetLength(unsigned char* pString) {
	unsigned int len = 0;
	for(;*pString!='\0';pString++, len++);
	return len;
}

/* 
Function to get int value of ascii character
Input: unsigned char 
Output: integer value
*/
unsigned int AsciiToInteger(unsigned char asciiVal) {
	unsigned int intVal = asciiVal - '0';

	return intVal;
}

/* 
Function to get num multiplied by 8 to the power of 
Input: number and 8 power value
Output: integer value
*/
unsigned int PowerOf8(unsigned int number, unsigned int pow) {
	unsigned int powerValue = number;
	if(number !=0) {
		if(pow > 0) {
			//for to the power times, multiply by 8
			for(; pow!=0; pow--) {
				//Multiply by 8 using left shift by 3 times
				powerValue = (powerValue << 3);
			}
		}
	}

	return powerValue;
}

/* 
Function to convert ungined octal string to unsigned integer value
Input: unsigned char string
Output: unsigned decimal value
Error Case: if the input string is invalid, returns 0
Error Case: if one of the digit in input string is not an octal number, returns 0
if the input string is too big for size of integer, which is 4 bytes, it overflows
*/
unsigned int OctalToDecimal(unsigned char* pOctalString) {
	unsigned int decimalVal = 0;
	unsigned int len;
	unsigned int index = 0;
	unsigned int digit = 0;
	/* Validate the input string */
	if(pOctalString) {
		len = GetLength(pOctalString);
		for(;index < len; index++) {
			digit = AsciiToInteger(pOctalString[index]);
			//Digit should not be more than 7 in octal system
			if(digit > 7) {
				decimalVal = 0;
				break;
			}
			decimalVal += PowerOf8(digit, len - index - 1);
		}
	}

	return decimalVal;
}
