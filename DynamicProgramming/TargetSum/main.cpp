/*
 * https://leetcode.com/problems/target-sum/
 *
 * You are given an integer array nums and an integer target.
 *
 * You want to build an expression out of nums by adding one of the symbols '+' and '-' before each integer in nums and then concatenate all the integers.
 * 
 * For example, if nums = [2, 1], you can add a '+' before 2 and a '-' before 1 and concatenate them to build the expression "+2-1".
 * Return the number of different expressions that you can build, which evaluates to target.
 *
 * Example 1:
 *  Input: nums = [1,1,1,1,1], target = 3
 *  Output: 5
 *  Explanation: There are 5 ways to assign symbols to make the sum of nums be target 3.
 *  -1 + 1 + 1 + 1 + 1 = 3
 *  +1 - 1 + 1 + 1 + 1 = 3
 *  +1 + 1 - 1 + 1 + 1 = 3
 *  +1 + 1 + 1 - 1 + 1 = 3
 *  +1 + 1 + 1 + 1 - 1 = 3
 */

#include <limits>
#include <vector>
#include <numeric>
#include <iostream>
#include <unordered_map>

void display(const char* what, const std::vector<int>& array) {
  std::cout << what << " is: ";

  for (auto item : array) {
    std::cout << item << " ";
  }

  std::cout << std::endl;
}

int findTargetSumWaysRecursive(const std::vector<int>& nums, int length, int target) {
  if (length < 0) {
    return 0;
  }
  if (length == 0) {
    if (target == 0) {
      return 1;
    }
    return 0;
  }

  int sum = findTargetSumWaysRecursive(nums, length - 1, target - nums[length - 1]);
  sum += findTargetSumWaysRecursive(nums, length - 1, target + nums[length - 1]);

  return sum;
}

int findTargetSumWaysRecursive(const std::vector<int>& nums, int target) {
  return findTargetSumWaysRecursive(nums, nums.size(), target);
}

int findTargetSumWaysRecursiveDP(const std::vector<int>& nums, int numsTotal, int length,
                                 int target, std::vector<std::vector<int>>& dp) {
  if (length < 0) {
    return 0;
  }
  if (length == 0) {
    if (target == 0) {
      return 1;
    }
    return 0;
  }

  if (dp[length - 1][target + numsTotal] != std::numeric_limits<int>::min()) {
    return dp[length - 1][target + numsTotal];
  }
  int sum = findTargetSumWaysRecursiveDP(nums, numsTotal, length - 1, target - nums[length - 1], dp);
  sum += findTargetSumWaysRecursiveDP(nums, numsTotal, length - 1, target + nums[length - 1], dp);

  dp[length - 1][target + numsTotal] = sum;
  return sum;
}

int findTargetSumWaysRecursiveDP(const std::vector<int>& nums, int target) {
  if (nums.empty()) {
    return 0;
  }
  int numsTotal = std::accumulate(nums.begin(), nums.end(), 0);
  if (std::abs(target) > numsTotal) {
    return  0;
  }

  // Initialize the DP with INT_MIN
  std::vector<std::vector<int>> dp(nums.size(), std::vector<int>(2 * numsTotal + 1, std::numeric_limits<int>::min()));
  return findTargetSumWaysRecursiveDP(nums, numsTotal, nums.size(), target, dp);
}

int findTargetSumWaysDP(const std::vector<int>& nums, int target) {
  if (nums.empty()) {
    return 0;
  }
  int numsTotal = std::accumulate(nums.begin(), nums.end(), 0);
  if (std::abs(target) > numsTotal) {
    return  0;
  }

  std::vector<int> dp(2 * numsTotal + 1, 0);
  //Initialize the dp
  dp[nums[0] + numsTotal] = 1;
  dp[-nums[0] + numsTotal] = 1;

  for (int i = 1; i < nums.size(); i++) {
    std::vector<int> next(2 * numsTotal + 1, 0);
    for (int sum = -numsTotal; sum <= numsTotal; sum++) {
      if (dp[sum + numsTotal] > 0) {
        next[sum + nums[i] + numsTotal] += dp[sum + numsTotal];
        next[sum - nums[i] + numsTotal] += dp[sum + numsTotal];
      }
    }
    dp = next;
  }

  return dp[target + numsTotal];
}

void testFindTargetSumWays(std::vector<int> nums, int target) {
  display("Input Vector", nums);
  auto ways1 = findTargetSumWaysRecursive(nums, target);
  std::cout << "No of ways " << target << " can be achieved is: " << ways1 << std::endl;
  auto ways2 = findTargetSumWaysRecursiveDP(nums, target);
  std::cout << "No of ways " << target << " can be achieved is: " << ways2 << std::endl;
  auto waysDP = findTargetSumWaysDP(nums, target);
  std::cout << "No of ways " << target << " can be achieved is: " << waysDP << std::endl;
}

int main() {
  testFindTargetSumWays({1,1,1,1,1}, 3);
  testFindTargetSumWays({1,2,3,5}, 5);

  return 0;
}
