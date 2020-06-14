/**
 * Longest common substring: https://www.youtube.com/watch?v=BysNXJHzCEs
 * a ==> "abcdaf"
 * b ==> "zbcdf"
 *
 * output ==> "bcd"
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


string LongestCommonSubString(const string& a, const string& b) {
  string subStr;
  size_t aLength = a.length() + 1;
  size_t bLength = b.length() + 1;
  size_t subStrRow = 0;
  size_t subStrCol = 0;
  size_t biggest = 0;

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
        if (dp[row][col] > biggest) {
          biggest = dp[row][col];
          subStrRow = row;
          subStrCol = col;
        }
      }
    }
  }

  // backtrack to get the subsequence.
  for (size_t row = subStrRow, col = subStrCol; dp[row][col] != 0; row--, col--) {
    subStr.insert(0, 1, a[row - 1]);
  }

  return subStr;
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

    string result = LongestCommonSubString(a, b);

    cout << "A: " << a << " B: " << b << " LongestCommonSubString: " << result << "\n";
  }

  return 0;
}
