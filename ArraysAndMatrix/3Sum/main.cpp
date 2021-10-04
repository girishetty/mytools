/*
 *
 *
 */

#include <vector>
#include <iostream>
#include <algorithm>

void displayVector(const std::vector<int>& nums) {
  std::cout << "[ ";
  for (auto item : nums) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
  std::vector<std::vector<int>> triplets;
  
  if (nums.size() < 3) {
    return triplets;
  }
  
  std::sort(nums.begin(), nums.end());

  for (int i = 0; i < nums.size() - 2; i++) {
    for (int j = i + 1; j < nums.size() - 1; j++) {
      auto sum = nums[i] + nums[j];
      auto lower = std::lower_bound(nums.begin() + j + 1, nums.end(), -sum);
      if (lower != nums.end()) {
        int start = std::distance(nums.begin(), lower);
        if (nums[start] == -sum) {
            triplets.push_back({nums[i], nums[j], nums[start]});
        }
      }

      // skip the duplicate elements
      for (; j < nums.size() - 1 && nums[j] == nums[j + 1]; j++);
    }

    // skip the duplicate elements
    for (; i < nums.size() - 1 && nums[i] == nums[i + 1]; i++);
  }
  
  return triplets;
}

std::vector<std::vector<int>> threeSumFaster(std::vector<int>& nums) {
  std::vector<std::vector<int>> triplets;
  int size = nums.size();
  
  if (size < 3) {
    return triplets;
  }
  
  std::sort(nums.begin(), nums.end());

  for (int i = 0; i < size - 2; i++) {
    int left = i + 1;
    int right = size - 1;
    while (left < right) {
      auto sum = nums[i] + nums[left] + nums[right];
      if (sum > 0) {
        right--;
      } else if (sum < 0) {
        left++; 
      } else {
        triplets.push_back({nums[i], nums[left], nums[right]});

        // skip the left duplicate elements
        for (; left < right && nums[left] == nums[left + 1]; left++);
        left++;

        // skip the right duplicate elements
        for (; left < right && nums[right] == nums[right - 1]; right--);
        right--;
      }
    }

    // skip the duplicate elements
    for (; i < size - 1 && nums[i] == nums[i + 1]; i++);
  }
  
  return triplets;
}

void testThreeSum(std::vector<int> nums) {
  std::cout << "Input Vector: ";
  displayVector(nums);

  //auto triplets = threeSum(nums);
  auto triplets = threeSumFaster(nums);

  std::cout << "Triplets: {" << std::endl;
  for (auto item : triplets) {
    displayVector(item);
  }

  std::cout << "}" << std::endl;
}

int main() {
  testThreeSum({-1,0,1,2,-1,-4});
  testThreeSum({0,0,0,0});
  testThreeSum({-2,0,1,1,2});

  return 0;
}
