/*
 * https://www.youtube.com/watch?v=_nCsPn7_OgI
 *
 * Find the Longest Palindromic Subsequence in a given string
 * For example, given abbdba, ouput should be: abdba
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

string longestPalindromeSubsequence(const string& input) {
  size_t inputSize = input.size();
  // When there is only one character in the input
  if (inputSize == 1){
    return input;
  }
  // Memoize vector that holds the size of the longest palindrome subsequence in the given string.
  // M[i][j] = size of longest palindrome subsequence
  // where i is the begin index and j end index of substring.
  vector<vector<size_t>> M(inputSize, vector<size_t>(inputSize, 0));

  // We start with substring of size 1
  // We know for a fact that, if there is only one element, then its already
  // palendrome.
  // So, M[0][0], M[1][1], M[2][2] .... M[inputSize-1][inputSize-1] is 1
  for (size_t index = 0; index < inputSize; index++) {
    M[index][index] = 1;
  }

  // Lets start with substring of size 2 till inputSize
  size_t length = 2;
  for (; length <= inputSize; length++) {
    size_t begin = 0;
    size_t end = begin + length - 1;
    for (; end < inputSize; begin++, end++) {
      // Check if the first and last characters are the same.
      if (input[begin] == input[end]) {
        // Now size of the longest palindrome subsequence should be
        // 2 + size of the longest palindrome subsequence from begin+1..end-1
        M[begin][end] = 2;
        size_t insideBegin = begin + 1;
        size_t insideEnd = end - 1;
        if (insideBegin <= insideEnd) {
          M[begin][end] += M[insideBegin][insideEnd];
        }
      } else {
        // The longest palindrome subsequence should be coming from either of
        // substring begin..end-1 OR begin+1..end
        M[begin][end] = std::max(M[begin][end - 1], M[begin + 1][end]);
      }
    }
  } 

#ifdef DEBUG
  printMatrix(M);
#endif

  // Now we have the size of the longest subsequence at M[0][inputSize-1].
  // We can back track from there to construct the palindrome subsequence.
  size_t begin = 0;
  size_t end = inputSize - 1;
  size_t palSize = M[begin][end];
  string palindrome(palSize, 0);
  size_t palBeginIndex = 0;
  size_t palEndIndex = palSize - 1;

  for (;  palBeginIndex <= palEndIndex;) {
    if (M[begin][end - 1] == M[begin][end]) {
      // Came from the substring begin..end-1
      end--;
    } else if (M[begin + 1][end] == M[begin][end]) {
      // Came from the substring begin+1..end
      begin++;
    } else {
      // We have got the 2 entries for the palindromic subsequence ==> input[begin] == input[end]
      palindrome[palBeginIndex++] = input[begin];
      palindrome[palEndIndex--] = input[begin];
      begin++;
      end--;
    }
  }

  return palindrome;
}

int main() {
  //string input("abcbd");
  string input("abbdba");
  cout << "Enter a string: ";
  cin >> input;
  auto pal = longestPalindromeSubsequence(input);
  cout << "Input: " << input << " Longest Palindrome Subsequence: " << pal << endl;

  return 0;
}


