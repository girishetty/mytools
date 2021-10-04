/*
 * https://leetcode.com/problems/regular-expression-matching/
 *
 * Given an input string s and a pattern p, implement regular expression matching with support for '.' and '*' where:
 *  - '.' Matches any single character.
 *  - '*' Matches zero or more of the preceding element.
 *
 * The matching should cover the entire input string (not partial).
 *
 * Example 1:
 *    Input: s = "aa", p = "a"
 *    Output: false
 *    Explanation: "a" does not match the entire string "aa".
 *
 *    Example 2:
 *    Input: s = "aa", p = "a*"
 *    Output: true
 *    Explanation: '*' means zero or more of the preceding element, 'a'. Therefore, by repeating 'a' once, it becomes "aa".
 *
 * Example 3:
 *    Input: s = "ab", p = ".*"
 *    Output: true
 *    Explanation: ".*" means "zero or more (*) of any character (.)".
 *
 * Example 4:
 *    Input: s = "aab", p = "c*a*b"
 *    Output: true
 *    Explanation: c can be repeated 0 times, a can be repeated 1 time. Therefore, it matches "aab".
 *
 * Example 5:
 *    Input: s = "mississippi", p = "mis*is*p*."
 *    Output: false
 */

#include <string>
#include <vector>
#include <iostream>

void display(const std::vector<std::vector<bool>>& matrix) {
  for (const auto& list : matrix) {
    for (auto item : list) {
      std::cout << item << "  ";
    }
    std::cout << std::endl;
  }
}

inline bool isMatchingChar(char a, char b) {
  return (a == b || b == '.');
}

bool isMatch(const std::string& s, const std::string& p) {
  size_t size_s = s.size();
  size_t size_p = p.size();

  if (size_p == 0) {
    // if pattern is empty, its a match!
    return true;
  }

  std::vector<std::vector<bool>> dp(size_p + 1, std::vector<bool>(size_s + 1, false));
  // When both are emptry
  dp[0][0] = true;

  // First col - when search is empty
  for (size_t row = 1; row <= size_p; row++) {
    if (p[row - 1] == '*') {
      // Not checking the index bound as its guaranteed that
      //for each appearance of the character '*', there will be a previous valid character to match.
      dp[row][0] = dp[row - 2][0];
    }
  }

  // Now when both aren't empty
  for (size_t col = 1; col <= size_s; col++) {
    size_t i = col - 1;
    for (size_t row = 1; row <= size_p; row++) {
      size_t j = row - 1;
      if (isMatchingChar(s[i], p[j])) {
        // matches if it matched so far (last row)
        dp[row][col] = dp[row - 1][col - 1];
      } else if (p[j] == '*') {
        // cases are:
        // 1. With out taking this character at all ( 2 rows earlier)
        // 2. With taking this character one time (last row)
        // 3. With taking this character multiple times, provided repeating patter matches the search pattern.
        dp[row][col] = dp[row - 2][col] || dp[row - 1][col] ||
        (isMatchingChar(s[i], p[j - 1]) && (dp[row - 1][col - 1] || dp[row][col - 1]));
      }
    }
  }
  
  return dp[size_p][size_s];
}

void testRegularExpression(std::string s, std::string p) {
  bool matching = isMatch(s, p);
  std::cout << "S: " << s << "  Pattern: " << p << " ";
  if (matching) {
    std::cout << "Match!" << std::endl;
  } else {
    std::cout << "Doesn't Match!" << std::endl;
  }
}

int main() {
  testRegularExpression("aa", "a");
  testRegularExpression("aa", "a*");
  testRegularExpression("ab", ".*");
  testRegularExpression("aab", "c*a*b");
  testRegularExpression("mississippi", "mis*is*p*.");
  testRegularExpression("aaa", "ab*a*c*a");
  testRegularExpression("aaba", "ab*a*c*a");
  testRegularExpression("aaa", ".*");

  return 0;
}
