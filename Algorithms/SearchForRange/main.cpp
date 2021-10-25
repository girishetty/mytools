/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/802/
 *
 * Given an array of integers nums sorted in non-decreasing order, find the starting and ending position of a given target value.
 *
 * If target is not found in the array, return [-1, -1].
 *
 * You must write an algorithm with O(log n) runtime complexity.
 *
 * Example 1:
 *    Input: nums = [5,7,7,8,8,10], target = 8
 *    Output: [3,4]
 *
 * Example 2:
 *    Input: nums = [5,7,7,8,8,10], target = 6
 *    Output: [-1,-1]
 *
 * Example 3:
 *    Input: nums = [], target = 0
 *    Output: [-1,-1]
 *
 */

#include <vector>
#include <iostream>

void display(const char* what, const std::vector<int>& arr) {
  std::cout << what << ": [";
  for (size_t i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << "]" << std::endl;
}

int findFirstOccurance(const std::vector<int>& nums, int target) {
  int left = 0;
  int right = nums.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    int diff = target - nums[mid];
    if (diff == 0 && (mid == 0 || nums[mid - 1] != target)) {
      return mid;
    }
    if (diff <= 0) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  return -1;
}

int findLastOccurance(const std::vector<int>& nums, int target) {
  int left = 0;
  int size = nums.size() - 1;
  int right = size;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    int diff = target - nums[mid];
    if (diff == 0 && (mid == size || nums[mid + 1] != target)) {
      return mid;
    }
    if (diff >= 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

std::vector<int> searchRange(const std::vector<int>& nums, int target) {
  std::vector<int> range(2, -1);
  range[0] = findFirstOccurance(nums, target);
  if (range[0] != -1) {
    range[1] = findLastOccurance(nums, target);
  }

  return range;
}

void testSearchRange(const std::vector<int>& nums, int target) {
  display("Input", nums);
  auto range = searchRange(nums, target);

  std::cout << target << "'s range in the input is: [" <<
               range[0] << ".." << range[1] << "]" << std::endl;
}

int main() {
  testSearchRange({5,7,7,8,8,10}, 8);
  testSearchRange({5,7,7,8,8,10}, 6);
  testSearchRange({}, 0);
  testSearchRange({1,2,3,3,3,3,3,3,3,4,5}, 3);
  testSearchRange({1,2,3,3,3,3,3,3,3,4,5}, 5);
}
