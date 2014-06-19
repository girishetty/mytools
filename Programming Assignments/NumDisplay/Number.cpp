#include <iostream>
#include <string>
#include "Number.h"
using namespace std;

const int KMaxDigits		= 10;

void fun(...)
{
}

Number::Number () : iNumber(0) { 
	iNumberString[0]='\0';
}

Number::Number (unsigned int aNumber) : iNumber(aNumber)  {
	iNumberString[0]='\0';
}

Number::Number (Number& aNumber) : iNumber(aNumber.iNumber) {
	strcpy (iNumberString, aNumber.iNumberString);
}


Number& Number::operator= (unsigned int& aNumber) {
	iNumber = aNumber;

	return *this;
}

Number& Number::operator= (Number& aNumber) {
	iNumber = aNumber.iNumber;
	strcpy (iNumberString, aNumber.iNumberString);

	return *this;
}

int Number::Compare (unsigned int& aNumber) {
	return ((iNumber == aNumber) ? 0 : (iNumber < aNumber) ? -1 : 1);
}

int Number::Compare (Number& aNumber) {
	return ((iNumber == aNumber.iNumber) ? 0 : (iNumber < aNumber.iNumber) ? -1 : 1);
}

void Number::ConvertToString () {
	unsigned int eachDigits [KMaxDigits];
	unsigned int totalDigits = GetEachDigits (eachDigits);

	if (totalDigits == 0) {
		strcpy(iNumberString, "Zero");
	}

	for (int i=totalDigits; i>0; i--) {
		unsigned int val;

		if (i == 3 || i == 1) { // If its Hundred or last digit.
			val = eachDigits[i-1];
			UpdateWithDigitValue (val);
			if (val != 0) {
				UpdateWithPositionValue (i);
			}
		}

		else if (i == 2) { // If its in 10th position
			val = eachDigits[i-1];

			if (val == 1) {
				val = val * 10 + eachDigits[--i-1];
			}
				
			UpdateWithTwoDigitValue (val);
		}

		else { // If its more than 100th position
			val = eachDigits[i-1];
			if (i % 2 == 1) {
				if (val == 1) {
					val = val * 10 + eachDigits[--i-1];
				}
				UpdateWithTwoDigitValue (val);
			}
			else {
				val = eachDigits[i-1];
				UpdateWithDigitValue (val);
			}

			if (val != 0) {
				UpdateWithPositionValue (i);
			}
		}
	}
}

unsigned int Number::GetEachDigits (unsigned int* aDigitsArray) {
	unsigned int number = iNumber;
	int index = 0;

	for( ;number != 0; number /= 10) {
		aDigitsArray [index++] = number % 10;
	}

	return index;
}

void Number::UpdateWithDigitValue (unsigned int aDigit) {

	switch(aDigit) {
	case 9:
		strcat(iNumberString, " Nine");
		break;
	case 8:
		strcat(iNumberString, " Eight");
		break;
	case 7:
		strcat(iNumberString, " Seven");
		break;
	case 6:
		strcat(iNumberString, " Six");
		break;
	case 5:
		strcat(iNumberString, " Five");
		break;
	case 4:
		strcat(iNumberString, " Four");
		break;
	case 3:
		strcat(iNumberString, " Three");
		break;
	case 2:
		strcat(iNumberString, " Two");
		break;
	case 1:
		strcat(iNumberString, " One");
		break;
	default:
		strcat(iNumberString, "");
	}
}

void Number::UpdateWithTwoDigitValue (unsigned int aDigit) {

	switch(aDigit) {
	case 19:
		strcat(iNumberString, " Nineteen");
		break;
	case 18:
		strcat(iNumberString, " Eighteen");
		break;
	case 17:
		strcat(iNumberString, " Seventeen");
		break;
	case 16:
		strcat(iNumberString, " Sixteen");
		break;
	case 15:
		strcat(iNumberString, " Fifteen");
		break;
	case 14:
		strcat(iNumberString, " Fourteen");
		break;
	case 13:
		strcat(iNumberString, " Thirteen");
		break;
	case 12:
		strcat(iNumberString, " Twelve");
		break;
	case 11:
		strcat(iNumberString, " Eleven");
		break;
	case 10:
		strcat(iNumberString, " Ten");
		break;
	case 9:
		strcat(iNumberString, " Ninety");
		break;
	case 8:
		strcat(iNumberString, " Eighty");
		break;
	case 7:
		strcat(iNumberString, " Seventy");
		break;
	case 6:
		strcat(iNumberString, " Sixty");
		break;
	case 5:
		strcat(iNumberString, " Fifty");
		break;
	case 4:
		strcat(iNumberString, " Forty");
		break;
	case 3:
		strcat(iNumberString, " Thirty");
		break;
	case 2:
		strcat(iNumberString, " Twenty");
		break;
	default:
		strcat(iNumberString, "");
	}
}

void Number::UpdateWithPositionValue (unsigned int aPosition) {
	
	switch(aPosition) {
	case 8:
		strcat(iNumberString, " Crore");
		break;
	case 6:
		strcat(iNumberString, " Lak");
		break;
	case 4:
		strcat(iNumberString, " Thousand");
		break;
	case 3:
		strcat(iNumberString, " Hundred");
		break;
	default:
		strcat(iNumberString, "");
	}
}

void Number::Display () {
	cout<<"Number is : "<<iNumber<<endl;
	cout<<"Number is : "<<iNumberString<<endl;
}

int main () {
	cout<<"Enter Some Positive Numbers "<<endl;
	cout<<"Press -1 to Terminate "<<endl;
	int n;

	do {
		cout<<"Enter a Number : ";
		cin>>n;

		Number num (n);
		num.ConvertToString ();
		num.Display ();
	}while(n != -1);

	return 0;
}
