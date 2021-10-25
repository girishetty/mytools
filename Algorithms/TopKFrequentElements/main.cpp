/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/799/
 *
 * Given an integer array nums and an integer k, return the k most frequent elements.
 * You may return the answer in any order.
 *
 * Example 1:
 *    Input: nums = [1,1,1,2,2,3], k = 2
 *    Output: [1,2]
 *
 * Example 2:
 *    Input: nums = [1], k = 1
 *    Output: [1]
 */

#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

void display(const char* what, const std::vector<int>& list) {
  std::cout << what << " is: [ ";
  for (auto item : list) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

/*
 * Variation of given problem where in we find all elements,
 * that appear K or more times in the list
 */
std::vector<int> getKOrMoreFrequent(const std::vector<int>& nums, int k) {
  // First build a map of item and their counts
  std::unordered_map<int, int> counts;
  for (auto item : nums) {
    counts[item]++;
  }

  // Now, build frequency list, in which frequency[index] is for list of
  // all items that appear in that list.
  // frequency[0] will be an empty list
  // At most, all elements can be unique. So, we will keep this list
  // of size 1 more than input list
  std::vector<std::vector<int>> frequencyList(nums.size() + 1);
  // Also maintain a list of all frequencies
  std::set<int> frequencySet;
  for (const auto& item : counts) {
    frequencyList[item.second].push_back(item.first);
    frequencySet.insert(item.second);
  }

  // Now look for all the items that appear K or more times
  auto found = frequencySet.lower_bound(k);
  std::vector<int> list;
  for (; found != frequencySet.end(); found++) {
    list.insert(list.end(), frequencyList[*found].begin(), frequencyList[*found].end());
  }

  return list;
}

/*
 * Return k most frequent elements from the given list
 */
std::vector<int> topKFrequent(const std::vector<int>& nums, int k) {
  // First build a map of item and their counts
  std::unordered_map<int, int> counts;
  for (auto item : nums) {
    counts[item]++;
  }

  // Now, build frequency list, in which frequency[index] is for list of
  // all items that appear in that list.
  // frequency[0] will be an empty list
  // At most, all elements can be unique. So, we will keep this list
  // of size 1 more than input list
  std::vector<std::vector<int>> frequencyList(nums.size() + 1);
  // Also maintain a list of all frequencies
  std::set<int> frequencySet;
  for (const auto& item : counts) {
    frequencyList[item.second].push_back(item.first);
    frequencySet.insert(item.second);
  }

  // Now start from the end to get K most frequent elements
  std::vector<int> list;
  for (auto index = frequencySet.crbegin(); k > 0 && index != frequencySet.crend(); index++) {
    const auto& items = frequencyList[*index];
    int toCopy = items.size();
    if (k < toCopy) {
        toCopy = k;
    }
    list.insert(list.end(), items.begin(), items.begin() + toCopy);
    // Now that we have copied toCopy elements, update k
    k -= toCopy;
  }

  return list;
}

void testTopKFrequent(const std::vector<int>& nums, int k) {
  auto list = topKFrequent(nums, k);
  display("Input", nums);
  char str[50];
  snprintf(str, 50, "%d most frequent elements", k);
  display(str, list);
  list = getKOrMoreFrequent(nums, k);
  snprintf(str, 50, "Elements appearing %d or more times", k);
  display(str, list);
  std::cout << "==============================================================================" << std::endl;
}

int main() {
  testTopKFrequent({1}, 1);
  testTopKFrequent({1,1,1,2,2,3}, 2);
  testTopKFrequent({0, 4, 5, 0, 1, 4, 1, 2, 3, 3, 2, 5, 4, 0, 6, 7, 6, 7}, 2);

  return 0;
}
