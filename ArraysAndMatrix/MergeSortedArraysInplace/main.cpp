/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/96/sorting-and-searching/587/
 *
 * You are given two integer arrays nums1 and nums2, sorted in non-decreasing order,
 * and two integers m and n, representing the number of elements in nums1 and nums2 respectively.
 *
 * Merge nums1 and nums2 into a single array sorted in non-decreasing order.
 * 
 * The final sorted array should not be returned by the function, but instead be stored inside
 * the array nums1. To accommodate this, nums1 has a length of m + n, where the first m elements
 * denote the elements that should be merged, and the last n elements are set to 0 and
 * should be ignored. nums2 has a length of n.
 *
 * Example#1:
 *    Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
 *    Output: [1,2,2,3,5,6]
 *
 * Example#2:
 *    Input: nums1 = [0], m = 0, nums2 = [1], n = 1
 *    Output: [1]
 *
 */

#include <vector>
#include <iostream>

void display(const char* name, const std::vector<int>& list) {
  std::cout << name << ": ";
  for (const auto& item : list) {
    std::cout << item << ' ';
  }
  std::cout << std::endl;
}

void merge(std::vector<int>& nums1, int m, const std::vector<int>& nums2, int n) {
  if (n == 0) {
    return;
  }
  if (m == 0) {
    nums1 = nums2;
    return;
  }

  int newSize = m + n - 1;
  m--;
  n--;
  for (; m >= 0 && n >= 0; newSize--) {
    if (nums1[m] > nums2[n]) {
      nums1[newSize] = nums1[m--];
    } else {
      nums1[newSize] = nums2[n--];
    }
  }

  while (n >= 0) {
    nums1[newSize--] = nums2[n--];
  }
}

int main() {
  std::vector<int> v1 {1, 2, 3};
  std::vector<int> v2 {2, 5, 6};

  display("Input vector#1", v1);
  display("Input vector#2", v2);
  int v1_size = v1.size();
  int v2_size = v2.size();

  // Extend v1 with enough space for v2
  v1.resize(v1_size + v2_size);

  // Now do sorted merge
  merge(v1, v1_size, v2, v2_size);
  display("Merged vector#1", v1);
  return 0;
}
