/**
 * Longest subsequence
 * a ==> "abcdaf"
 * b ==> "acbcf"
 *
 * output ==> a, b, c, f
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

inline void Display2DVector(const vector<vector<size_t>>& dp) {
  for (auto rows : dp) {
    for (auto item : rows) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
  }
  cout << "================================" << endl;
}


string LongestSubsequence(const string& a, const string& b) {
  string subsequence;
  size_t aLength = a.length() + 1;
  size_t bLength = b.length() + 1;

  // size_t array [a.length() + 1][b.length() + 1]
  // Initialize the array with 0 length, which covers the case of a or b empty.
  vector<vector<size_t>> dp(aLength, vector<size_t>(bLength, 0));

  // Loop through the elements and fill in dp matrix
  for (size_t row = 1; row < aLength; row++) {
    size_t aIndex = row - 1;

    for (size_t col = 1; col < bLength; col++) {
      size_t bIndex = col - 1;

      if (a[aIndex] == b[bIndex]) {
        dp[row][col] = dp[row - 1][col - 1] + 1;
      } else {
        dp[row][col] = max(dp[row - 1][col], dp[row][col - 1]);
      }
    }
    cout << endl;
  }

  // backtrack to get the subsequence.
  for (size_t row = aLength - 1; row > 0;) {
    for (size_t col = bLength - 1; col > 0;) {
      if (dp[row - 1][col - 1] == dp[row - 1][col] && dp[row - 1][col] == dp[row][col - 1]) {
        // move diagonally and know that this is part of subsequence.
        row--;
        col--;
        subsequence.insert(0, 1, a[row]);
      } else if (dp[row][col] == dp[row][col - 1]) {
        // move left
        col--;
      } else {
        // move up
        row--;
      }
    }
  }

  Display2DVector(dp);
  cout << dp[aLength - 1][bLength - 1] << endl;
  return subsequence;
}

int main() {
  int q;
  cin >> q;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  for (int q_itr = 0; q_itr < q; q_itr++) {
    string a;
    getline(cin, a);

    string b;
    getline(cin, b);

    string result = LongestSubsequence(a, b);

    cout << "A: " << a << " B: " << b << endl << "LongestSubsequence: " << result << "\n";
  }

  return 0;
}
