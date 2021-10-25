/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/111/dynamic-programming/807/
 *
 * You are given an integer array nums. You are initially positioned at the array's first index,
 * and each element in the array represents your maximum jump length at that position.
 * Return true if you can reach the last index, or false otherwise.
 *
 * Example 1:
 *    Input: nums = [2,3,1,1,4]
 *    Output: true
 *    Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
 *
 * Example 2:
 *    Input: nums = [3,2,1,0,4]
 *    Output: false
 *    Explanation: You will always arrive at index 3 no matter what. Its maximum jump length is 0, which makes it impossible to reach the last index.
 *
 */

#include <vector>
#include <iostream>

bool canJump(const std::vector<int>& steps) {
  int size = steps.size();
  std::vector<bool> dp(size, false);
  // When we are at the last step.
  dp[size - 1] = true;

  // Now start from the last but one step and see if we can reach the last step.
  // Repeat it, until we reach the first step.
  for (int index = size - 2; index >= 0; index--) {
    int maxJumps = index + steps[index];
    for (int jump = index + 1; jump <= maxJumps; jump++) {
      dp[index] = dp[jump];
      if (dp[index]) {
        break;
      }
    }
  }

  return dp[0];
}

inline void display(const char* what, const std::vector<int>& arr) {
  std::cout << what << "is: [";
  for (auto item : arr) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

void testCanJump(const std::vector<int>& steps) {
  display("Steps", steps);
  bool can = canJump(steps);
  if (can) {
    std::cout << "Can reach the last index" << std::endl;
  } else {
    std::cout << "Can't reach the last index" << std::endl;
  }
}

int main() {
  testCanJump({2, 3, 1, 1, 4});
  testCanJump({3, 2, 1, 0, 4});
  return 0;
}
