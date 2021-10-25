/*
 * https://www.youtube.com/watch?v=_nCsPn7_OgI
 *
 * Find the Longest Palindromic Substring in a given string
 * A palindrome is a sequence of characters that reads the same backwards and forwards.
 * Given a string, s, find the longest palindromic substring in s.
 *
 * Example:
 * Input: "banana"
 * Output: "anana"
 *
 * Input: "million"
 * Output: "illi"
 *
 * Input: "anda"
 * Output: "a"
 */

#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

using std::cin;
using std::endl;
using std::cout;
using std::setw;
using std::string;
using std::vector;

void printMatrix(const vector<vector<size_t>>& matrix) {
  cout << endl << "=======================================" << endl;
  for (const auto& row : matrix) {
    for (auto item : row) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
  }
}

string longestPalindromeSubstring(const string& input) {
  size_t inputSize = input.size();
  // When there is only one character in the input
  if (inputSize == 1){
    return input;
  }
  // Memoize vector that holds the size of the longest palindrome substring in the given string.
  // M[i][j] = size of longest palindrome substring
  // where i is the begin index and j end index of substring.
  vector<vector<size_t>> M(inputSize, vector<size_t>(inputSize, 0));

  // We start with substring of size 1
  // We know for a fact that, if there is only one element, then its already
  // palendrome.
  // So, M[0][0], M[1][1], M[2][2] .... M[inputSize-1][inputSize-1] is 1
  for (size_t index = 0; index < inputSize; index++) {
    M[index][index] = 1;
  }

  // A structure to carry the info about longest substring palindrome found so far.
  struct LongestSubstr {
    size_t len = 1;
    size_t beginIndex = 0;
  } longestSubstr;

  // Lets start with substring of size 2 till inputSize
  size_t length = 2;
  for (; length <= inputSize; length++) {
    size_t begin = 0;
    size_t end = begin + length - 1;
    for (; end < inputSize; begin++, end++) {
      // Check if the first and last characters are the same.
      if (input[begin] == input[end]) {
        // This could be a potential palindromic substring with length: length.
        M[begin][end] = length;
        // See if the substring from input[begin+1..end-1] is also a palindrome.
        size_t insideBegin = begin + 1;
        size_t insideEnd = end - 1;
        if (insideBegin <= insideEnd && M[insideBegin][insideEnd] == 0) {
          // inside substring isn't a palindrome. So this is also not a palindrome.
          M[begin][end] = 0;
        } else if (M[begin][end] > longestSubstr.len) {
          longestSubstr.len = length;
          longestSubstr.beginIndex = begin;
        }
      }
    }
  }

#ifdef DEBUG
  printMatrix(M);
#endif

  return input.substr(longestSubstr.beginIndex, longestSubstr.len);
}

int main() {
  //string input("abcbd");
  string input("abbdba");
  cout << "Enter a string: ";
  cin >> input;
  auto pal = longestPalindromeSubstring(input);
  cout << "Input: " << input << " Longest Palindrome Substring: " << pal << endl;

  return 0;
}


