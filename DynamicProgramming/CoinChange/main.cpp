/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/111/dynamic-programming/807/
 *
 * You are given an integer array coins representing coins of different denominations and
 * an integer amount representing a total amount of money.
 *
 * Return the fewest number of coins that you need to make up that amount.
 * If that amount of money cannot be made up by any combination of the coins, return -1.
 *
 * You may assume that you have an infinite number of each kind of coin.
 *
 * Example 1:
 *    Input: coins = [1,2,5], amount = 11
 *    Output: 3
 *    Explanation: 11 = 5 + 5 + 1
 *
 */

#include <vector>
#include <iostream>
#include <algorithm>

inline void display(const char* what, const std::vector<int>& arr) {
  std::cout << what << " are: [ ";
  for (auto item : arr) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

int coinChange(std::vector<int>& coins, int amount) {
  // Sort the coins in descending order
  std::sort(coins.begin(), coins.end(), std::greater<int>());
  std::vector<int> dp(amount + 1, -1);
  dp[0] = 0;

  for (int i = 1; i <= amount; i++) {
    for (auto coin : coins) {
      if (coin <= i) {
        if (i == coin) {
          // Only one coin is needed to getting this amount
          dp[i] = 1;
        } else if (dp[i - coin] > 0) {
          // 1 more coin is needed than "i - coin".
          int count = 1 + dp[i - coin];
          // Check if this is fewer than whatever we have already counted
          if (dp[i] == -1 || count < dp[i]) {
            dp[i] = count;
          }
        }
      } else {
        continue;
      }
    }
  }

  return dp[amount];
}

void testCoinChange(const std::vector<int>& coins, int amount) {
  display("Given Coins", coins);
  std::vector<int> inputCoins = coins;
  int count = coinChange(inputCoins, amount);
  std::cout << count << " number of coins will make up the amount: " << amount << std::endl;
}

int main() {
  testCoinChange({2, 5}, 20);
  testCoinChange({1, 2, 5}, 11);
  testCoinChange({1, 2, 5}, 49);
  testCoinChange({1}, 0);
  testCoinChange({1}, 1);
  testCoinChange({2}, 3);
  testCoinChange({186, 419, 83, 408}, 6249);
  return 0;
}
