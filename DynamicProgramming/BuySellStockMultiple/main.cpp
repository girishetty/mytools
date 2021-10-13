/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/92/array/564/
 *
 * On each day, you may decide to buy and/or sell the stock.
 * You can only hold at most one share of the stock at any time.
 * However, you can buy it then immediately sell it on the same day.
 *
 * Find and return the maximum profit you can achieve.
 *
 * Example 1:
 *  Input: prices = [7,1,5,3,6,4]
 *  Output: 7
 *  Explanation: Buy on day 2 (price = 1) and sell on day 3 (price = 5), profit = 5-1 = 4.
 *  Then buy on day 4 (price = 3) and sell on day 5 (price = 6), profit = 6-3 = 3.
 *  Total profit is 4 + 3 = 7.
 *
 */

#include <vector>
#include <climits>
#include <iostream>

void display(const char* what, const std::vector<int>& list) {
  std::cout << what << " is: [";
  for (const auto& item : list) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

int maxProfitFaster(const std::vector<int>& prices) {
  int totalProfit = 0;
  int saleProfit = 0;
  int buyingDay = 0;
  int n = prices.size();

  for (int i = 0; i < n-1; i++) {
    // If tomorrow's price is less than today's
    if (prices[i+1] <= prices[i]) {
      // Take the profit by selling the shares today (as the price is going to drop tomorrow)
      totalProfit += saleProfit;
      // We want to buy again tomorrow, as the price is going to go down
      buyingDay = i+1;
      // So reset the profit - as we just sold and starting with clean slate
      saleProfit = 0;
    } else {
      // Since tomorrow's price is better than today,
      // see how much profit we will make if we sell it tomorrow
      saleProfit = prices[i+1] - prices[buyingDay];
    }
  }

  if (saleProfit > 0) {
    totalProfit += saleProfit;
  }
  return totalProfit;
}

int maxProfit(const std::vector<int>& prices) {
  if (prices.size() < 2) {
    return 0;
  }

  // DP of minimum buying price, last sold price, and the profit
  struct Transaction {
    int profit = 0;
    int lastBuyingPrice = 0;
    int lastSoldPrice = INT_MAX;

    void set(int prof, int buy = 0, int sell = INT_MAX) {
      profit = prof;
      lastBuyingPrice = buy;
      lastSoldPrice = sell;
    }
  };
  std::vector<Transaction> dp(prices.size());
  int profit = prices[1] - prices[0];
  bool sold = false;
  if (profit > 0) {
    // Buy on day#1 and Sell it on day#2
    sold = true;
    dp[1].set(profit, prices[0], prices[1]);
  } else {
    // We aren't buying yet, but looking at day#2 as possible buying day
    dp[1].set(0, prices[1]);
  }

  int i = 2;
  for (; i < prices.size(); i++) {
    int profit = 0;
    if (!sold) {
      profit = prices[i] - dp[i-1].lastBuyingPrice;
    }
    if (profit > 0) {
      // We have got a new selling price, as the profit is better now
      dp[i].set(dp[i - 1].profit + profit, dp[i - 1].lastBuyingPrice, prices[i]);
      sold = true;
    } else if (prices[i] > dp[i-1].lastSoldPrice) {
      // We have got a better selling price, amend the last sell and sell it now
      profit = prices[i] - dp[i-1].lastSoldPrice;
      dp[i].set(dp[i-1].profit + profit, dp[i-1].lastBuyingPrice, prices[i]);
      sold = true;
    } else if (prices[i] < dp[i-1].lastSoldPrice) {
      // Time to buy again
      dp[i].set(dp[i-1].profit, prices[i]);
      sold = false;
    } else if (prices[i] < dp[i-1].lastBuyingPrice) {
      // We have got a better buying price, amend the last buy and make a new buy
      dp[i].set(dp[i-1].profit, prices[i]);
      sold = false;
    } else {
      // No change
      dp[i] = dp[i-1];
    }
  }
  
  return dp[i - 1].profit > 0 ? dp[i - 1].profit : 0;
}

void testGetMaxProfit(const std::vector<int> prices) {
  display("Stock Price", prices);
  auto profit = maxProfit(prices);
  std::cout << "Max Profit through DP: " << profit << std::endl;
  profit = maxProfitFaster(prices);
  std::cout << "Max Profit through Quick: " << profit << std::endl;
}

int main() {
  testGetMaxProfit({3, 2, 6, 5, 0, 3});
  testGetMaxProfit({7, 1, 5, 3, 6, 4});
  testGetMaxProfit({1, 4, 2});
  testGetMaxProfit({1, 2, 3, 4, 5});
  testGetMaxProfit({7, 6, 4, 3, 1});
}
