/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/103/array-and-strings/781/
 *
 * Given an integer array nums, return true if there exists a triple of indices (i, j, k)
 * such that i < j < k and nums[i] < nums[j] < nums[k]. If no such indices exists, return false.
 *
 * Example#1:
 *    Input: nums = {1,2,3,4,5}
 *    Output: true
 *
 * Example#2:
 *    Input:  nums = {5,4,3,2,1}
 *    Output: false
 *
 * Example#3:
 *    Input:  nums = {2,1,5,0,4,6}
 *    Output: true
 *
 */

#include <vector>
#include <climits>
#include <iostream>

bool increasingTriplet(const std::vector<int>& nums, std::vector<int>& triplets) {
  triplets.clear();
  if (nums.size() < 3) {
    return false;
  }

  int smallest = INT_MAX;
  int nextSmallest = INT_MAX;

  for (auto num : nums) {
    if (num <= smallest) {
      smallest = num;
    } else if (num <= nextSmallest) {
      nextSmallest = num;
    } else {
      triplets.push_back(smallest);
      triplets.push_back(nextSmallest);
      triplets.push_back(num);
      return true;
    }
  }

  return false;
}

void display(const char* what, const std::vector<int>& nums) {
  std::cout << what << " is: [";

  for (auto item : nums) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

void testIncreasingTriplet(const std::vector<int>& nums) {
  display("Input", nums);
  std::vector<int> triplets;
  auto found = increasingTriplet(nums, triplets);
  if (found) {
    std::cout << "Found increasing triplets {" << triplets[0] << ", " << triplets[1] << ", " << triplets[2] << "} in the given input" << std::endl;
  } else {
    std::cout << "Did not find increasing triplets in the given input" << std::endl;
  }
}

int main() {
  testIncreasingTriplet({1, 2, 3, 4, 5});
  testIncreasingTriplet({5, 4, 3, 2, 1});
  testIncreasingTriplet({2, 1, 5, 0, 4, 6});

  return 0;
}
