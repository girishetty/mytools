/*
 * https://www.youtube.com/watch?v=lDYIvtBVmgo
 *
 * Find the minimum number of partition to create palindromes in a given string
 * For example, given abcbd, ouput should be: 2 ==> (a, bcb, d)
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

bool isPalindrome(const string& input, size_t begin, size_t end) {
  for (; begin < end && input[begin] == input[end]; begin++, end--);

  return input[begin] == input[end];
}

size_t makePalindromePartition(const string& input) {
  size_t inputSize = input.size();
  // Memoize vector that holds how many splits are needed to create
  // palindomes.
  // M[i][j] = split count.
  // where i is the begin index and j end index.
  vector<vector<size_t>> M(inputSize, vector<size_t>(inputSize, 0));

  // We start with spliting the input into inputSize number of items.
  // i.e break them into palindromes of length 1.
  // We know for a fact that, if there is only one element, then its already
  // palendrome.
  // So, M[0][0], M[1][1], M[2][2] .... M[inputSize-1][inputSize-1] is 0
  // i.e 0/NO split is needed to make them palindrome.
  size_t length = 2;
  for (; length <= inputSize; length++) {
    size_t begin = 0;
    size_t end = begin + length - 1;
    for (; end < inputSize; begin++, end++) {
      // If its already a palindrome, we don't have to split it.
      if (isPalindrome(input, begin, end)) {
        continue;
      }
      // Since its not a palindrome, lets split it by length: starting from start till end.
      // for size in range 0 to length - 1, find a split which is the minimum
      //   minSplit = Min Of {input[begin..begin+size] and input[begin+size+1..end]}
      size_t minSplit = (size_t)-1;
      for (size_t splitEnd = begin + 1; splitEnd <= end; splitEnd++) {
        auto splitCount = M[begin][splitEnd - 1] + M[splitEnd][end];
        if (minSplit > splitCount) {
          minSplit = splitCount;
        }
        // If we have already reached the best case scenario, i.e spliting this way already produces
        // 2 palindromes, then we can break the loop.
        if (minSplit == 0) {
          break;
        }
      }
      M[begin][end] = 1 + minSplit;
    }
#ifdef DEBUG
    printMatrix(M);
#endif
  } 
  return M[0][inputSize-1];
}

int main() {
  string input("abcbd");
  cout << "Enter a string: ";
  cin >> input;
  cout << "Input: " << input << " No of splits: " << makePalindromePartition(input) << endl;

  return 0;
}


