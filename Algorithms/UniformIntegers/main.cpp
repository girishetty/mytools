/*
 A positive integer is considered uniform if all of its digits are equal.
 For example, 222 is uniform, while 223 is not.

 Given two positive integers A and B, determine the number of uniform integers between A and B, inclusive.

 Constraints:
     1 ≤ A ≤ B ≤ 10 power 12

 Sample Test Case #1
     A = 75
     B = 300
     Expected Return Value = 5

 Sample Test Case #2
     A = 1
     B = 9
     Expected Return Value = 9A

 Sample Test Case #3
     A = 999999999999
     B = 999999999999
     Expected Return Value = 1

 Sample Explanation
     In the first case, the uniform integers between 75 and 300 are 77, 88, 99, 111, and 222.
     In the second case, all 9 single-digit integers between 1 and 9 (inclusive) are uniform.
     In the third case, the single integer under consideration 999,999,999,999 is uniform.
 */

#include <bits/stdc++.h>
using namespace std;

void display(const vector<int>& D) {
  std::cout << "Stack: ";
  for (auto item : D) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

/*
 Arithmetically, the number of digits of a positive integer N is d=⌊log10⁡N⌋+1
 For example, log 10 3918287712≈9.5931. So 3918287712 has ⌊9.5931⌋+1=10 digits.

 Once you know the number of digits d of N, you can find the first digit
 f of N by calculating f=⌊N/10 power (d−1)⌋.
 For example, the first digit of 3918287712 is ⌊3918287712/10 power 10−1⌋=⌊3.918287712⌋=3
*/

size_t getFirstDigitAndNoOfDigits(long long number, int& firstDigit) {
  size_t noOfDigits = std::floor(std::log10(number)) + 1;

  firstDigit = floor(number / std::pow(10, noOfDigits - 1));

  return noOfDigits;
}

long long getUniformNumber(int digit, size_t noOfDigits) {
  long long firstUniformNumber = digit;

  while (noOfDigits > 1) {
    firstUniformNumber = firstUniformNumber * 10 + digit;
    noOfDigits--;
  }

  return firstUniformNumber;
}

int getUniformIntegerCountInInterval(long long A, long long B) {
  int    firstDigit1 = 0;
  size_t noOfDigits1 = getFirstDigitAndNoOfDigits(A, firstDigit1);
  int    firstDigit2 = 0;
  size_t noOfDigits2 = getFirstDigitAndNoOfDigits(B, firstDigit2);

  if (noOfDigits1 == noOfDigits2 && noOfDigits2 == 1) {
    std::cout << "[" << A << " .. " << B << "]: " << B - A + 1 << std::endl;
    return B - A + 1;
  }

  long long firstUniformNumber = getUniformNumber(firstDigit1, noOfDigits1);
  long long lastUniformNumber = getUniformNumber(firstDigit2, noOfDigits2);

  int uniformNumbers = 10 - firstDigit1;

  if (A > firstUniformNumber) {
    uniformNumbers--;
  }

  auto diff = noOfDigits2 - noOfDigits1;
  if (diff > 2) {
    uniformNumbers = uniformNumbers + 9 * (diff - 1);
  }

  if (diff > 0) {
    uniformNumbers += firstDigit2;
  } else {
    uniformNumbers -= (9 - firstDigit2);
  }

  if (B < lastUniformNumber) {
    uniformNumbers--;
  }

  std::cout << "[" << A << " .. " << B << "]: " << uniformNumbers << std::endl;
  return uniformNumbers;
}

int main() {
  std::cout << getUniformIntegerCountInInterval(89, 998) << std::endl;
  std::cout << getUniformIntegerCountInInterval(89, 999) << std::endl;
  std::cout << getUniformIntegerCountInInterval(8, 8) << std::endl;
  std::cout << getUniformIntegerCountInInterval(1, 9) << std::endl;
  std::cout << getUniformIntegerCountInInterval(11, 88) << std::endl;
  std::cout << getUniformIntegerCountInInterval(75, 300) << std::endl;
  std::cout << getUniformIntegerCountInInterval(999999999999, 999999999999) << std::endl;
  std::cout << getUniformIntegerCountInInterval(9999999999997, 9999999999998) << std::endl;

  return 0;
}

