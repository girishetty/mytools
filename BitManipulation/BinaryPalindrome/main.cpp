#include <bitset>
#include <iostream>

//
// Write a function that returns true if binary representation of number x is a palindrome.
// For example (1000...001) is palindrome
//

/*
 * isPalindrome: implementation 1
 *
 * Devide and Conquer:
 * - Compare 4 bits (nibbles) at a time for palindrome
 * - Compare those nibbles against a predefined set
 */
bool isPalindrome_v4(int number) {
  static uint8_t pal_map[16] = { /*0b0000*/ 0b0000,
                                 /*0b0001*/ 0b1000,
                                 /*0b0010*/ 0b0100,
                                 /*0b0011*/ 0b1100,
                                 /*0b0100*/ 0b0010,
                                 /*0b0101*/ 0b1010,
                                 /*0b0110*/ 0b0110,
                                 /*0b0111*/ 0b1110,
                                 /*0b1000*/ 0b0001,
                                 /*0b1001*/ 0b1001,
                                 /*0b1010*/ 0b0101,
                                 /*0b1011*/ 0b1101,
                                 /*0b1100*/ 0b0011,
                                 /*0b1101*/ 0b1011,
                                 /*0b1110*/ 0b0111,
                                 /*0b1111*/ 0b1111};

  bool result = true;
  size_t shift_count = (sizeof(int) * 8) - 4; // 8 as bits in a byte
  int lsbMask = 0x0F;
  int msbMask = 0x0F << shift_count;
  int left = number;
  int right = number;

  for (size_t count = 0; count < 4; count++) {
    auto rightNibble = right & lsbMask;
    auto leftNibble = ((left & msbMask) >> shift_count ) & lsbMask;
    if (pal_map[rightNibble] != leftNibble) {
      result = false;
      break;
    }
    right = right >> 4;
    left = left << 4;
  }
  return result;
}

/*
 * isPalindrome: implementation 3
 *
 * Compare individual bits from left and right one at a time.
 */
bool isPalindrome_v3(int number) {
  bool result = true;
  size_t no_of_bits = sizeof(int) * 8; // 8 as bits in a byte
  int lsbMask = 0x01;
  int msbMask = 0x01 << (no_of_bits - 1);
  int left = number;
  int right = number;

  for (; right != 0 && left != 0; ) {
    auto lsb = right & lsbMask;
    auto msb = ((left & msbMask) >> (no_of_bits - 1)) & lsbMask;
    if (lsb != msb) {
      result = false;
      break;
    }
    right = right >> 1;
    left = left << 1;
  }
  return result;
}

// This function returns true if k'th bit in x is set (or 1).
// For example if x (0010) is 2 and k is 2, then it returns true
// NOTE that it makes an assumption that 1 <= k <= 32
inline bool isKthBitSet(int x, size_t k) {
  return (x & (1 << (k - 1))) ? true : false;
}
 
/*
 * isPalindrome: implementation 2
 *
 * Compare individual bits from left and right one at a time.
 */
bool isPalindrome_v2(int x) {
  int l = 1; // Initialize left position
  int r = sizeof(int) * 8; // initialize right position
 
  // One by one compare bits
  while (l < r) {
    if (isKthBitSet(x, l) != isKthBitSet(x, r)) {
      return false;
    }
    l++; r--;
  }
  return true;
}
 
/*
 * isPalindrome: implementation 1
 *
 * Generate the binary reverse of the given number and check if both are the same.
 */
bool isPalindrome_v1(unsigned int n) {
  // `rev` stores reverse of a binary representation of `n`
  unsigned int rev = 0;
 
  // do till all bits of `n` are processed
  unsigned int k = n;
  size_t count = 0;
  while (k) {
    // add the rightmost bit to `rev`
    rev = (rev << 1) | (k & 1);
    k = k >> 1;     // drop last bit
    count++;
  }
 
  // Take care of the leading 0's
  size_t bitCount = sizeof(int) * 8;
  if (count < bitCount) {
    rev = rev << (bitCount - count);
  }

  // Returns true if `reverse(n)` is the same as `n`
  return n == rev;
}
 
/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/99/others/565/
 *
 * Write a function that takes an unsigned integer and returns the number of '1' bits it has
 * (also known as the Hamming weight).
 */

int hammingWeight(uint32_t n) {
  // No of bits for each nibble from 0 to 15
  static uint8_t nibble_bits_count[16] = {
         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
  int count = 0;
  do {
    auto nibble = n & 0xF;
    count += nibble_bits_count[nibble];
    n = n >> 4;
  } while (n != 0);
  
  return count;
}

/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/99/others/648/
 *
 * Reverse bits of a given 32 bits unsigned integer.
 */
uint32_t reverseBits(uint32_t n) {
  size_t l = sizeof(uint32_t) << 3;
  uint32_t reversed = 0;

  for (size_t r = 1; r <= l; r++) {
    bool rSet = isKthBitSet(n, r);
    reversed = reversed << 1;
    if (rSet) {
      reversed = reversed | 0x1;
    }
  }

  return reversed;
}

int main() {
  int n = 285212808;
  std::cout << "Enter a Number: ";
  std::cin >> n;

  std::cout << n << "[" << std::bitset<32>(n) << "] :" << std::endl;
  if (isPalindrome_v1(n)) {
    std::cout << "- is a binary palindrome" << std::endl;
  } else {
    std::cout << "- is not a binary palindrome" << std::endl;
  }

  if (isPalindrome_v2(n)) {
    std::cout << "- is a binary palindrome" << std::endl;
  } else {
    std::cout << "- is not a binary palindrome" << std::endl;
  }

  if (isPalindrome_v3(n)) {
    std::cout << "- is a binary palindrome" << std::endl;
  } else {
    std::cout << "- is not a binary palindrome" << std::endl;
  }

  if (isPalindrome_v4(n)) {
    std::cout << "- is a binary palindrome" << std::endl;
  } else {
    std::cout << "- is not a binary palindrome" << std::endl;
  }
  return 0;
}
