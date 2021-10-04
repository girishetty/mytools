#include <iostream>
#include <string>
#include "Number.h"
using namespace std;

const int KMaxDigits    = 10;

Number::Number() : iNumber(0) { 
  iNumberString[0]='\0';
}

Number::Number(unsigned int aNumber) : iNumber(aNumber)  {
  iNumberString[0]='\0';
}

Number::Number(const Number& aNumber) : iNumber(aNumber.iNumber) {
  strcpy(iNumberString, aNumber.iNumberString);
}


Number& Number::operator=(unsigned int& aNumber) {
  iNumber = aNumber;
  return *this;
}

Number& Number::operator=(const Number& aNumber) {
  iNumber = aNumber.iNumber;
  strcpy (iNumberString, aNumber.iNumberString);
  return *this;
}

int Number::Compare(unsigned int& aNumber) {
  return ((iNumber == aNumber) ? 0 : (iNumber < aNumber) ? -1 : 1);
}

int Number::Compare(const Number& aNumber) {
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
    } else if (i == 2) { // If its in 10th position
      val = eachDigits[i-1];
      if (val == 1) {
        val = val * 10 + eachDigits[--i-1];
      }
      UpdateWithTwoDigitValue (val);
    } else { // If its more than 100th position
      val = eachDigits[i-1];
      if (i % 2 == 1) {
        if (val == 1) {
          val = val * 10 + eachDigits[--i-1];
        }
        UpdateWithTwoDigitValue (val);
      } else {
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

  char* digitArray[] = { "", " One", " Two", " Three", " Four", " Five", " Six", " Seven", " Eight", " Nine" };

  strcat(iNumberString, digitArray[aDigit]);
}

void Number::UpdateWithTwoDigitValue (unsigned int aDigit) {

  char* digitArray[] = { "", "", " Twenty", " Thirty", " Forty", " Fifty", " Sixty", " Seventy", " Eighty", " Ninety", " Ten",
            " Eleven", " Twelve", " Thirteen", " Fourteen", " Fifteen", " Sixteen", " Seventeen", " Eighteen", " Nineteen" };

  strcat(iNumberString, digitArray[aDigit]);
}

void Number::UpdateWithPositionValue (unsigned int aPosition) {

  char* position[] = { "", "", "", " Hundred", " Thousand", "", " Lak", "", " Crore", "" };
  strcat(iNumberString, position[aPosition]);
}

void Number::Display () const {
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

    if (n==-1) {
      break;
    }
    Number num (n);
    num.ConvertToString ();
    num.Display ();
  } while(n != -1);

  return 0;
}
