/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/97/dynamic-programming/576/
 *
 * You are a professional robber planning to rob houses along a street. Each house has a certain
 * amount of money stashed, the only constraint stopping you from robbing each of them is that
 * adjacent houses have security systems connected and it will automatically contact the police
 * if two adjacent houses were broken into on the same night.
 *
 * Given an integer array nums representing the amount of money of each house,
 * return the maximum amount of money you can rob tonight without alerting the police.
 *
 * Example#1:
 *    Input: nums = {1, 2, 3, 1}
 *    Output: 4
 *    Explanation: Rob house 1 (money = 1) and then rob house 3 (money = 3).
 *    Total amount you can rob = 1 + 3 = 4.
 *
 * Example#2:
 *    Input: {2, 7, 1, 3, 8}
 *    Output:
 *    Explanation: Rob house 2 (money = 7) and then rob house 5 (money = 8).
 *    Total amount you can rob = 7 + 8 = 15.
 */

#include <vector>
#include <iostream>

void display(const char* what, const std::vector<int>& array) {
  std::cout << what << " is: ";
  for (auto item : array) {
    std::cout << item << "  ";
  }

  std::cout << std::endl;
}

int rob(const std::vector<int>& houses) {
  int numHouses = houses.size();
  std::vector<int> dp(numHouses + 1, 0);
  if (numHouses > 1) {
    dp[1] = houses[0];
    dp[2] = houses[1] > houses[0] ? houses[1] : houses[0];
  } else if (numHouses > 0) {
    dp[1] = houses[0];
  }

  for (int i = 2; i < numHouses; i++) {
    // At this point we have 2 options:
    // 1. Rob this house along with previous to previous house
    // 2. Don't rob this house, and settle with the previous house rob.
    // Pick whichever yields more
    int option1 = dp[i - 1] + houses[i];
    dp[i + 1] = option1 > dp[i] ? option1 : dp[i];
  }

  return dp[numHouses];
}

void testRob(const std::vector<int>& houses) {
  display("Houses with Money", houses);
  int maxMoney = rob(houses);
  std::cout << "Robbed Max of $" << maxMoney << std::endl;
}

int main() {

  testRob({1, 2, 3, 1});
  testRob({2, 7, 1, 3, 8});
  testRob({2, 7, 9, 3, 1});
  return 0;
}
