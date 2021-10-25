/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/111/dynamic-programming/808/
 *
 * A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).
 *
 * The robot can only move either down or right at any point in time. The robot is trying to reach
 * the bottom-right corner of the grid.
 *
 * How many possible unique paths are there?
 *
 * Example#1:
 *    Input: m = 3, n = 2
 *    Output: 3
 *    Explanation: From the top-left corner, there are a total of 3 ways to reach the bottom-right corner:
 *      1. Right -> Down -> Down
 *      2. Down -> Down -> Right
 *      3. Down -> Right -> Down
 *
 * Example#2:
 *    Input: m = 3, n = 7
 *    Output: 28
 */

#include <vector>
#include <iostream>

int gridTraveller(int m, int n, std::vector<std::vector<int>>& dp) {
  if (m < 0 || n < 0) {
    // We have reached no return path
    return 0;
  }
  if (m == 0 && n == 0) {
    // We have already reached the corner
    return 1;
  }

  if (dp[m][n] != -1) {
    return dp[m][n];
  }

  // We can either go down or go right from given spot
  dp[m][n] = gridTraveller(m - 1, n, dp) + gridTraveller(m, n - 1, dp);
  return dp[m][n];
}

int gridTraveller(int m, int n) {
  // a dp of size m * n
  // Alternatively we can use a map ==> std::map<std::string, int> dp
  // where in key will be "m + n"
  std::vector<std::vector<int>> dp(m, std::vector<int>(n, -1));
  // Since we are 0 indexing, lets start with m - 1 and n - 1
  return gridTraveller(m - 1, n - 1, dp);
}

void testGridTraveller(int m, int n) {
  int noOfWays = gridTraveller(m, n);
  std::cout << m << " * " << n << " Grid can be Travelled in " << noOfWays << " unique ways" << std::endl;
}

int main() {
  testGridTraveller(3, 2);
  testGridTraveller(3, 2);
  testGridTraveller(3, 7);
  return 0;
}
