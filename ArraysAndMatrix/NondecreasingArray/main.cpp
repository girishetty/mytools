/*
 https://leetcode.com/problems/non-decreasing-array/

 Given an array nums with n integers, your task is to check if it could become non-decreasing by modifying at most one element.
 We define an array is non-decreasing if nums[i] <= nums[i + 1] holds for every i (0-based) such that (0 <= i <= n - 2).

 Example:
    Input: nums = [4,2,3]
    Output: true
    Explanation: You could modify the first 4 to 1 to get a non-decreasing array.


    Input: nums = [4,2,1]
    Output: false
    Explanation: You can't get a non-decreasing array by modify at most one element.
 */
#include <bits/stdc++.h>

void display(const std::vector<int>& D) {
  std::cout << "List: ";
  for (auto item : D) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

bool checkPossibility(std::vector<int> nums) {
  display(nums);
  bool modifiedOneTime = false;
  for (int index = 0 ; index < nums.size() - 1; index++) {
    std::cout << index << ": [" << nums[index] << ":" << nums[index+1] << "]" << std::endl;
    if (nums[index] <= nums[index+1]) {
      continue;
    }
    if (modifiedOneTime) {
      std::cout << "Fail me" << std::endl;
      return false;
    }
    modifiedOneTime = true;
    if (index == 0) {
      std::cout << "Modifying " << nums[0] << " to " << nums[1] << std::endl;
      nums[0] = nums[1];
    } else {
      int i = index + 2;
      if (i < nums.size() && nums[i] < nums[index]) {
        std::cout << "Modifying " << nums[index] << " to " << nums[index - 1] << std::endl;
        nums[index] = nums[index - 1];
      } else {
        std::cout << "Modifying " << nums[index + 1] << " to " << nums[index] << std::endl;
        nums[index + 1] = nums[index];
      }
      index--;
    }
  }
    
  return true;
}

int main() {
  std::cout << checkPossibility({-1, 4, 2, 3}) << std::endl;
  std::cout << checkPossibility({1, 1, 2, 3, 1, 6}) << std::endl;
  std::cout << checkPossibility({1, 1, 2, 3, 2, 2}) << std::endl;
  std::cout << checkPossibility({1, 1, 2, 3, 2, 1}) << std::endl;
  std::cout << checkPossibility({3, 4, 2, 3}) << std::endl;

  return 0;
}

