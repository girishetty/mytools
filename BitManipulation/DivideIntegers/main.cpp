/*
 https://leetcode.com/problems/divide-two-integers/

 Given two integers dividend and divisor, divide two integers without using multiplication, division, and mod operator.
 Return the quotient after dividing dividend by divisor.
 The integer division should truncate toward zero, which means losing its fractional part. For example, truncate(8.345) = 8 and truncate(-2.7335) = -2.

 Note: Assume we are dealing with an environment that could only store integers within the 32-bit signed integer range: [−231, 231 − 1]. For this problem, assume that your function returns 231 − 1 when the division result overflows.
 */


#include <iostream>

size_t getShiftCount(int number) {
  size_t shiftCount = 0;
  for (size_t bit = 0; bit < 32; bit++) {
    if (number & (1 << bit)) {
      return bit;
    }
  }

  return 0;
}

int divide(int dividend, int divisor) {
  bool positive = true;
  bool addOne = false;
  int result = 0;
  
  if (dividend < 0) {
    positive = !positive;
    if (dividend == -2147483648) {
      dividend = 2147483647;
      addOne = true;
    } else {
      dividend = -dividend;
    }
  } else if (dividend == 0) {
    return 0;
  }

  if (divisor < 0) {
    positive = !positive;
    if (divisor == -2147483648) {
      if (addOne) {
        return positive ? 1 : -1;
      }
      return 0;
    }
    divisor = -divisor;
  }
  
  if ((divisor & (divisor - 1)) == 0) {
    size_t shiftCount = getShiftCount(divisor);
    result = dividend >> shiftCount;
    dividend = dividend & (divisor - 1);
  } else {
    while (dividend >= divisor) {
      dividend = dividend - divisor;
      result++;
    }
  }

  if (addOne) {
    dividend++;
    addOne = false;
    if (dividend >= divisor) {
      addOne = true;   
    }
  }
  
  result = positive ? result : -result;
  if (addOne) {
    if (result < 2147483647 && result > 0) {
      result++;
    } else if (result < 0 && result > -2147483648) {
      result--;
    }
  }
  return result;
}

int main() {
  int a = -2147483648;
  int b = 1;
  std::cout << a << " / " << b << " = " << divide(a, b) << std::endl;
  return 0;
}

