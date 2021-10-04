/*
 * https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
 *
 * You are given an array prices where prices[i] is the price of a given stock on the ith day.
 * You want to maximize your profit by choosing a single day to buy one stock and choosing a different day in the future to sell that stock.
 * Return the maximum profit you can achieve from this transaction. If you cannot achieve any profit, return 0.
 *
 * Example 1:
 *  Input: prices = [7,1,5,3,6,4]
 *  Output: 5
 *  Explanation: Buy on day 2 (price = 1) and sell on day 5 (price = 6), profit = 6-1 = 5.

 *  Note that buying on day 2 and selling on day 1 is not allowed because you must buy before you sell.
 */
#include <vector>
#include <iostream>

int maxProfit(std::vector<int> prices) {
  if (prices.size() < 2) {
    return 0;
  }

  // DP of minimum price and the profit
  std::vector<std::pair<int, int>> dp(prices.size(), {0, 0});
  int profit = prices[1] - prices[0];
  if (profit > 0) {
    dp[1] = {prices[0], profit};
  } else {
    dp[1] = {prices[1], 0};
  }

  int i = 2;
  for (; i < prices.size(); i++) {
    int profit = prices[i] - dp[i-1].first;
    if (profit > dp[i-1].second) {
      // We have got a new selling price, as the profit is better now
      dp[i] = std::make_pair(dp[i - 1].first, profit);
    } else if (prices[i] < dp[i-1].first) {
      // We have got a new buying price, though current profit is the best profit
      dp[i] = std::make_pair(prices[i], dp[i-1].second);
    } else {
      // No change
      dp[i] = dp[i-1];
    }
  }
  
  return dp[i - 1].second > 0 ? dp[i - 1].second : 0;
}

int main() {
  std::cout << "Max Profit: " << maxProfit({3,2,6,5,0,3}) << std::endl;
  std::cout << "Max Profit: " << maxProfit({7,1,5,3,6,4}) << std::endl;
  std::cout << "Max Profit: " << maxProfit({1,4,2}) << std::endl;
}

