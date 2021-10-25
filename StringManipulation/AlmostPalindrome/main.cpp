/*
 * https://leetcode.com/problems/valid-palindrome-ii/
 *
 * Given a string s, return true if the s can be palindrome after deleting at most one character from it.
 */
#include <string>
#include <vector>
#include <iostream>

inline bool isPalindrome(const std::string& s, size_t& front, size_t& back) {
  for (; front < back; front++, back--) {
    if (s[front] != s[back]) {
      return false;
    }
  }

  return true;
}

bool validPalindrome(const std::string& s) {
  size_t front = 0;
  size_t back = s.size() - 1;
  
  if (isPalindrome(s, front, back)) {
    return true;
  }

  size_t newFront = front + 1;
  size_t newBack = back - 1;
  return isPalindrome(s, front, newBack) || isPalindrome(s, newFront, back);
}

void testValidPalindrome(std::string input) {
  std::cout << "String: " << input << std::endl;
  std::cout << "validPalindrome: " << validPalindrome(input) << std::endl;
}

int main() {
  testValidPalindrome("lcupuuffuupucul");
  validPalindrome("aguokepatgbnvfqmgmlcupuufxoohdfpgjdmysgvhmvffcnqxjjxqncffvmhvgsymdjgpfdhooxfuupuculmgmqfvnbgtapekouga");
  testValidPalindrome("cbbcc");
  testValidPalindrome("acxcybycxcxa");

  return 0;
}
