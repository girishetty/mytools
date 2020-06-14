/**
 * https://www.hackerrank.com/challenges/abbr/problem?h_l=interview&playlist_slugs%5B%5D=interview-preparation-kit&playlist_slugs%5B%5D=dynamic-programming
 */

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

#ifdef SOLUTION_ONE
string abbreviation(const string& a, const string& b) {
  if (a.empty()) {
   // If both are empty then f(a,b) = true
    if (b.empty()) {
      return "YES";
    }
    // If a is empty and b is non-empty, then f(a,b) is always false.
    return "NO";
  }
  if (b.empty()) {
    // If a is non-empty and b is empty, then f(a,b) is true only if all the characters in a are lower case.
    auto found = find_if(a.begin(), a.end(), [](char a) { return isupper(a); } );
    if (found == a.end()) {
      return "YES";
    }
    return "NO";
  }

  // Now start from the begining:
  // if a[aIndex] == b[bIndex], increment both indices
  // else if islower(a[aIndex])
  //    if (toupper(a[aIndex]) == b[bIndex]), increment a index , set a flag
  //    else if we have seen an equivalent lowercase letter earlier, increment b index and reset the flag.
  //    else increment only a index
  // else if we have seen an equivalent lowercase letter earlier, increment b index and reset the flag.
  // else return "NO"
  bool usefulLowerCase = false;
  size_t aIndex = 0;
  size_t bIndex = 0;
  for (; aIndex < a.size() && bIndex < b.size(); ) {
    if (a[aIndex] == b[bIndex]) {
      aIndex++;
      bIndex++;
    } else if (islower(a[aIndex])) {
      if (toupper(a[aIndex]) == b[bIndex]) {
        aIndex++;
        usefulLowerCase = true;
      } else if (usefulLowerCase) {
        bIndex++;
        usefulLowerCase = false;
      } else {
        aIndex++;
      }
    } else if (usefulLowerCase) {
        bIndex++;
        usefulLowerCase = false;
    } else {
      return "NO";
    }
  }

  if (bIndex == b.size()) {
    // Check if we have hit end of a also.
    if (aIndex == a.size()) {
      return "YES";
    }
    // Rest of a should be all lower case letters.
    auto found = find_if(a.begin() + aIndex, a.end(), [](char a) { return isupper(a); } );
    if (found == a.end()) {
      return "YES";
    }
  }
  return "NO";
}
#else
string abbreviation(const string& a, const string& b) {
  // bool array [a.length() + 1][b.length() + 1]
  vector<vector<bool>> dp(a.length() + 1);
  for (auto row = 0; row <= a.length(); row++) {
    dp[row].resize(b.length() + 1);
  }

  // case#1: when both a and b are empty.
  dp[0][0] = true;

  // case#2: a is non-empty and b is empty, then dp is true only if all the characters in a are lower case.
  bool foundUppercase = false;
  for (size_t row = 1; row <= a.length(); row++) {
    size_t aIndex = row - 1;

    if (foundUppercase || isupper(a[aIndex])) {
      // We have found a uppercase a[index]
      foundUppercase = true;
      dp[row][0] = false;
    } else {
      // We haven't found a uppercase a[index] yet
      dp[row][0] = true;
    }
  }

  // case#3: other cases. fill in dp matrix
  for (size_t row = 1; row <= a.length(); row++) {
    size_t aIndex = row - 1;

    for (size_t col = 1; col <= b.length(); col++) {
      size_t bIndex = col - 1;

      if (a[aIndex] == b[bIndex]) {
        dp[row][col] = dp[row - 1][col - 1];
      } else if (isupper(a[aIndex])) {
        dp[row][col] = false;
      } else if (toupper(a[aIndex]) == b[bIndex]) {
        dp[row][col] = dp[row-1][col-1] || dp[row-1][col];
      }  else {
        dp[row][col] = dp[row-1][col];
      }
    }
  }

  return dp[a.length()][b.length()] ? "YES" : "NO";
}

#endif

int main() {
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string a;
        getline(cin, a);

        string b;
        getline(cin, b);

        string result = abbreviation(a, b);

        cout << result << "\n";
    }

    return 0;
}
