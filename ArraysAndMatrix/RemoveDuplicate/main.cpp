/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/92/array/727/
 *
 * Given an integer array nums sorted in non-decreasing order, remove the duplicates in-place such that
 * each unique element appears only once. The relative order of the elements should be kept the same.
 *
 * Since it is impossible to change the length of the array in some languages, you must instead have the
 * result be placed in the first part of the array nums.
 * More formally, if there are k elements after removing the duplicates, then the first k elements
 * of nums should hold the final result. It does not matter what you leave beyond the first k elements.
 *
 * Return k after placing the final result in the first k slots of nums.
 *
 * Do not allocate extra space for another array. You must do this by modifying the input array in-place with O(1) extra memory.
 *
 * Example#1:
 * Input: nums = [1,1,2]
 * Output: 2, nums = [1,2,_]
 *
 * Example#2:
 * Input: nums = [0,0,1,1,1,2,2,3,3,4]
 * Output: 5, nums = [0,1,2,3,4,_,_,_,_,_]
 *
 */

#include <vector>
#include <iostream>

int removeDuplicates(std::vector<int>& nums) {
  int total = nums.size();
  if (total == 0) {
    return 0;
  }

  int curIndex = 0;
  int nextUniqueIndex = 0;

  for (; nextUniqueIndex < total; ) {
    // Move until we find the next number (bigger than the current number)
    for (; nextUniqueIndex < total && nums[curIndex] == nums[nextUniqueIndex]; nextUniqueIndex++);
    if (nextUniqueIndex == total) {
      // Nothing else to be done
      break;
    }
    nums[++curIndex] = nums[nextUniqueIndex];
  }
  
  return curIndex + 1;
}

void display(const char* what, const std::vector<int>& list) {
  std::cout << what << " is: [ ";
  for (const auto& item : list) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

void testRemoveDuplicates(std::vector<int> nums) {
  display("Sorted Input List", nums);
  int k = removeDuplicates(nums);
  nums.resize(k);
  display("Uniquely Sorted Input List", nums);
}

int main() {
  testRemoveDuplicates({1, 1, 2});
  testRemoveDuplicates({0, 0, 1, 1, 1, 2, 2, 3, 3, 4});

  return 0;
}
