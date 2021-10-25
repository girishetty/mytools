/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/801/
 *
 * A peak element is an element that is strictly greater than its neighbors.
 *
 * Given an integer array nums, find a peak element, and return its index.
 * If the array contains multiple peaks, return the index to any of the peaks.
 *
 * You may imagine that nums[-1] = nums[n] = -∞.
 *
 * You must write an algorithm that runs in O(log n) time.
 *
 * Example 1:
 *    Input: nums = [1,2,3,1]
 *    Output: 2
 *    Explanation: 3 is a peak element and your function should return the index number 2.
 *
 * Example 2:
 *    Input: nums = [1,2,1,3,5,6,4]
 *    Output: 5
 *    Explanation: Your function can return either index number 1 where the peak element is 2, or index number 5 where the peak element is 6.
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

int findPeakElement(const std::vector<int>& nums, int low, int high, int size) {
  int mid = low + (high -low) / 2;

  // Check if the mid element is peak.
  if ((mid == 0 || nums[mid] > nums[mid - 1]) && (mid == size - 1 || nums[mid] > nums[mid + 1])) {
    return mid;
  }

  // Check if the 1st half of the list (left of mid) has a peak.
  if (mid > 0 && nums[mid] <= nums [mid - 1]) {
    return findPeakElement(nums, low, mid - 1, size);
  }
  // Check if the 2nd half of the list (right of mid) has a peak.
  return findPeakElement(nums, mid + 1, high, size);
}
    
int findPeakElement(const std::vector<int>& nums) {
  int size = nums.size();
  return findPeakElement(nums, 0, size - 1, size);
}

int testFindPeakElement(const std::vector<int>& nums) {
  display("Input", nums);
  int pi = findPeakElement(nums);
  std::cout << "Peak Index is: " << pi << " Peak element: " << nums[pi] << std::endl;
}

int main() {
  testFindPeakElement({1,2,3,1});
  testFindPeakElement({1,2,1,3,5,6,4});

  return 0;
}
