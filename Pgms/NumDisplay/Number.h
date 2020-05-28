#ifndef __NUMBER_H__
#define __NUMBER_H__

const int KMaxStrLen		= 254;

class Number {
public:
	Number ();
	Number (unsigned int aNumber);
	Number (Number& aNumber);

	Number& operator=(unsigned int& aNumber);
	Number& operator=(Number& aNumber);

	int Compare(unsigned int& aNumber);
	int Compare(Number& aNumber);

	void ConvertToString ();
	unsigned int GetEachDigits(unsigned int* aDigitsArray);
	void UpdateWithDigitValue (unsigned int aDigit);
	void UpdateWithTwoDigitValue (unsigned int aDigit);
	void UpdateWithPositionValue (unsigned int aPosition);

	void Display ();

private:
	unsigned int iNumber;
	char iNumberString [KMaxStrLen];
};

#endif //__NUMBER_H__