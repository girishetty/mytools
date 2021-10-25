/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/798/
 *
 * Given an array nums with n objects colored red, white, or blue, sort them in-place
 * so that objects of the same color are adjacent, with the colors in the order red, white, and blue.
 *
 * We will use the integers 0, 1, and 2 to represent the color red, white, and blue, respectively.
 * You must solve this problem without using the library's sort function.
 *
 * Example 1:
 *    Input: nums = [2,0,2,1,1,0]
 *    Output: [0,0,1,1,2,2]
 *
 * Example 2:
 *    Input: nums = [2,0,1]
 *    Output: [0,1,2]
 *
 * Example 3:
 *    Input: nums = [0]
 *    Output: [0]
 *
 * Example 4:
 *    Input: nums = [1]
 *    Output: [1]
 *
 * Constraints:
 *    n == nums.length
 *    1 <= n <= 300
 *    nums[i] is 0, 1, or 2.
 *
 */

#include <vector>
#include <iostream>

void display(const char* what, const std::vector<int>& nums) {
  std::cout << what << " is [ ";
  for (auto item : nums) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

inline void fillColors(std::vector<int>& nums, int color, int startIndex, int endIndex) {
  for (; startIndex < endIndex; startIndex++) {
    nums[startIndex] = color;
  }
}

void sortColors(std::vector<int>& nums) {
  // Count of 0, 1 and 2
  std::vector<int> counts (3, 0);

  for (auto item : nums) {
    counts[item]++;
  }

  int start = 0;
  int end = counts[0];
  fillColors(nums, 0, start, end);

  start = end;
  end += counts[1];
  fillColors(nums, 1, start, end);

  start = end;
  end += counts[2];
  fillColors(nums, 2, start, end);
}

void testSortColors(const std::vector<int>& nums) {
  std::vector<int> numbers = nums;
  display("Input", numbers);
  sortColors(numbers);
  display("Sorted", numbers);
}

int main() {
  testSortColors({2,0,2,1,1,0});
  testSortColors({2,0,1});
  testSortColors({0});

  return 0;
}
