#ifndef __NUMBER_H__
#define __NUMBER_H__

const size_t KMaxStrLen = 254;

class Number {
public:
  Number();
  Number(unsigned int aNumber);
  Number(const Number& aNumber);

  Number& operator=(unsigned int& aNumber);
  Number& operator=(const Number& aNumber);

  int Compare(unsigned int& aNumber);
  int Compare(const Number& aNumber);

  void ConvertToString ();
  unsigned int GetEachDigits(unsigned int* aDigitsArray);
  void UpdateWithDigitValue (unsigned int aDigit);
  void UpdateWithTwoDigitValue (unsigned int aDigit);
  void UpdateWithPositionValue (unsigned int aPosition);

  void Display() const;

private:
  size_t iNumber;
  char iNumberString[KMaxStrLen];
};

#endif //__NUMBER_H__
