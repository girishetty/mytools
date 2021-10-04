/*
 * http://buttercola.blogspot.com/2018/10/leetcode-523-continuous-subarray-sum.html
 *
 * Given a list of non-negative numbers and a target integer k, write a function to check if the array has a continuous
 * subarray of size at least 2 that sums up to the multiple of k, that is, sums up to n*k where n is also an integer.
 *
 * Example 1:
 *    Input: [23, 2, 4, 6, 7],  k=6
 *    Output: True
 *    Explanation: Because [2, 4] is a continuous subarray of size 2 and sums up to 6.
 *
 * Example 2:
 *    Input: [23, 2, 6, 4, 7],  k=6
 *    Output: True
 *    Explanation: Because [23, 2, 6, 4, 7] is an continuous subarray of size 5 and sums up to 42.
 *
 * Note:
 *  - The length of the array won't exceed 10,000.
 *  - You may assume the sum of all the numbers is in the range of a signed 32-bit integer.
 */

#include <vector>
#include <numeric>
#include <iostream>
#include <unordered_map>

/*
 * To have a O(n) solution, there is a trick to consider. If
 *    a % k == c &&
 *    b % k == c
 *    Then (a - b) % k == 0. 
 *
 * Proof:
 *   Let a = m * k + c,
 *       b = n * k + c, then
 *       a - b = (m - n) * k, which modular k is 0.
 */
bool checkSubarraySum(const std::vector<int>& nums, int K) {
  size_t size = nums.size();

  // error condition
  if (size < 2) {
    return false;
  }

  std::vector<int> sumUpTo(size);
  sumUpTo[0] = nums[0];

  for (int i = 1; i < size; i++) {
    sumUpTo[i] = sumUpTo[i - 1] + nums[i];
  }

  std::unordered_map<int, int> map;
  map[0] = -1;

  for (int i = 1; i < size; i++) {
    int mod_residue = sumUpTo[i];
    if (K != 0) {
      mod_residue = mod_residue % K;
    }

    // See if this mod_residue (c) has been found already
    auto found = map.find(mod_residue);
    if (found != map.end()) {
      // Make sure it was at least 2 distance away (for at least 2 elements)
      if (i - found->second > 1) {
        return true;
      }
    } else {
      map[mod_residue] = i;
    }
  }

  return false;
}

void display(const char* what, const std::vector<int>& array) {
  std::cout << what << " is: ";
  for (auto item : array) {
    std::cout << item << "  ";
  }

  std::cout << std::endl;
}

void testCheckSubarraySum(std::vector<int> nums, int K) {
  bool possible = checkSubarraySum(nums, K);

  display("Input Number Array", nums);
  std::cout << "For given K as " << K << ", it is ";
  if (!possible) {
    std::cout << "not ";
  }
  std::cout << "Possible to find a Subarray in the above vector" << std::endl;
}

int main() {
  testCheckSubarraySum({23, 2, 4, 6, 7}, 56);
  testCheckSubarraySum({23, 2, 4, 6, 7}, 6);
  testCheckSubarraySum({2, 4, 5, 7}, 6);
  testCheckSubarraySum({2, 4, 7}, 6);
  testCheckSubarraySum({2, 4}, 6);
  testCheckSubarraySum({23, 2, 6, 4, 7}, 6);

  return 0;
}

