/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/804/
 *
 * There is an integer array nums sorted in ascending order (with distinct values).
 *
 * Prior to being passed to your function, nums is possibly rotated at an unknown pivot index k
 * (1 <= k < nums.length) such that the resulting array is
 * [nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]] (0-indexed).
 * For example, [0,1,2,4,5,6,7] might be rotated at pivot index 3 and become [4,5,6,7,0,1,2].
 *
 * Given the array nums after the possible rotation and an integer target, return the index of
 * target if it is in nums, or -1 if it is not in nums.
 *
 * You must write an algorithm with O(log n) runtime complexity.
 *
 * Example 1:
 *    Input: nums = [4,5,6,7,0,1,2], target = 0
 *    Output: 4
 *
 * Example 2:
 *    Input: nums = [4,5,6,7,0,1,2], target = 3
 *    Output: -1
 *
 * Example 3:
 *    Input: nums = [1], target = 0
 *    Output: -1
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

int findPartition(const std::vector<int>& nums) {
  int size = nums.size() - 1;
  int left = 0;
  int right = size;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    
    if (mid == size || nums[mid] > nums[mid + 1]) {
      return mid;
    }
    if (nums[mid] < nums[mid + 1]) {
      if (nums[mid] < nums[0]) {
        // The smallest element is on left half
        right = mid - 1;
      } else {
        // The smallest element is on right half
        left = mid + 1;
      }
    }
  }

  return -1;
}

int searchItem(const std::vector<int>& nums, int left, int right, int target) {
  while (left <= right) {
    int mid = left + (right - left) / 2;
    int diff = target - nums[mid];
    if (diff == 0) {
      return mid;
    }
    if (diff > 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

int searchRotatedList(const std::vector<int>& nums, int target) {
  int part1Size = findPartition(nums);
  int index = 0;
  if (target >= nums[0]) {
    // It should be in first partition
    index = searchItem(nums, 0, part1Size, target);
  } else {
    // It should be in the second partition
    index = searchItem(nums, part1Size + 1, nums.size() - 1, target);
  }

  return index;
}

void testSearchRotatedList(const std::vector<int>& nums, int target) {
  display("Input", nums);
  auto index = searchRotatedList(nums, target);

  std::cout << target << " is found at the index: " << index << std::endl;
}

int main() {
  testSearchRotatedList({4, 5, 6, 7, 0, 1, 2}, 0);
  testSearchRotatedList({4, 5, 6, 7, 0, 1, 2}, 3);
  testSearchRotatedList({5, 6, 0, 1, 2, 3, 4}, 3);
  testSearchRotatedList({6, 0, 1, 2, 3, 4, 5}, 0);
  testSearchRotatedList({1, 2, 3, 4, 5, 6, 0}, 0);
  testSearchRotatedList({1}, 1);

  return 0;
}
