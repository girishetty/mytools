/*
 * Problem:
 * 10. The standard library function of atoi() is not very robust.  
 * How would you design/implement a better version of it?
 * Solution:
 * Get the integral value of the string passed, if possible.
 * Else, if argument is invalid (NULL or non digit character in the begining of the string passed) return 0.
 * Else if the correct value is out of the range then return INT_MAX or INT_MIN.
 * This handles only decimal numbers
 */

/* Maximum signed integer value */
#define INT_MAX      0x7FFFFFFF
/* Minimum signed integer value */
#define INT_MIN      0x80000000

int atoi(const char* str) {
	int intVal = 0;
	int prevIntVal = 0;
	int digit = 0;
	int index = 0;
	int sign = 1;
	bool overflow = false;

	if(str) {
		/* skip all white spaces/tabs */
		for(;str[index]==' ';index++);
		
		/*look for sign character now*/
		if(str[index] = '-') {
			sign = -1;
			index++;
		}
		else if(str[index] = '+') {
			index++;
		}
		/* now start processing the number */
		for(;str[index]!='\0';index++) {
			/* subtract with ascii value of '0' to get the digit*/
			digit = str[index] - '0';
			/* validate the output for range of digits/numbers */
			if(digit >=0 && digit <= 9) {
				intVal = intVal*10 + digit;
				if(intVal < prevIntVal) {
					/* we have reached overflow now, so break the loop */
					overflow = true;
					break;
				}
				prevIntVal = intVal;
			}
			else {
				/* we have hit invalid character, so break the loop */
				break;
			}
		}
	}

	/* handle overflow now, before returning */
	if(overflow) {
		intVal = (sign == 1? INT_MAX : INT_MIN);
	}
	/* handle negetive number now, before returning */
	else if(sign == -1) 
		intVal = -intVal;
	}

	return intVal;
}
