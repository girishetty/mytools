/**
 * https://www.youtube.com/watch?v=nYFd7VHKyWQ
 *
 */

#define FASTER

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

void makePermutation(const string& input, vector<size_t>& counter,
                     string& result, size_t level,
                     vector<string>& list) {
  if (level == result.size()) {
    list.push_back(result);
    return;
  }

  for (size_t index = 0; index < input.size(); index++) {
    if (counter[index] == 0) {
      continue;
    }
    result[level] = input[index];
    counter[index]--;
    makePermutation(input, counter, result, level + 1, list);
    counter[index]++;
  }
}

/*
 * Given a string, return all the possible permutations.
 * You can return the answer in any order.
 */
vector<string> permutation(const string& A) {
  // Using map to find all unique characters and their occurances (count)
  unordered_map<char, size_t> countMap;
  std::for_each(A.begin(), A.end(), [&countMap](char ch) { countMap[ch]++; });

  string input;
  vector<size_t> counter;
  std::for_each(countMap.begin(), countMap.end(), [&input, &counter](const pair<char, size_t>& item) {
    input.push_back(item.first);
    counter.push_back(item.second);
  });

  string result(A.size(), '\0');
  vector<string> list;
  // Now lets create permutations of this input
  makePermutation(input, counter, result, 0, list);
  return list;
}

/*
 * Given an array nums of distinct integers, return all the possible permutations.
 * You can return the answer in any order.
 */
void makePermutation(const vector<int>& nums, vector<int>& counter,
                     vector<int>& result, int level,
                     vector<vector<int>>& list) {
  if (level == nums.size()) {
    list.push_back(result);
  }

  for (int index = 0; index < nums.size(); index++) {
    if (counter[index] == 0) {
      //We have alreay used this in the permutation
      continue;
    }
    counter[index] = 0;
    result[level] = nums[index];
    makePermutation(nums, counter, result, level + 1, list);
    counter[index] = 1;
  }
}

vector<vector<int>> permutation(const vector<int>& nums) {
  vector<vector<int>> list;
  vector<int> result(nums.size(), 0);
  vector<int> counter(nums.size(), 1);
  makePermutation(nums, counter, result, 0, list);
  return list;
}

void display(const vector<int>& nums) {
  std::cout << "{ ";
  for (auto item : nums) {
    std::cout << item << " ";
  }
  std::cout << "}" << std::endl;
}

void testPermutation(const vector<int>& nums) {
  auto list = permutation(nums);
  cout << "Permutations of ";
  display(nums);

  for (const auto& item : list) {
    display(item);
  }
}

int main() {
  string A("aab");
  cout << "Enter a string for which you are looking for possible Permutations: ";
  cin >> A;

  auto list = permutation(A);
  cout << "Permutations of " << A << " are: " << endl;
  for (const auto& item : list) {
    std::cout << item << std::endl;
  }

  testPermutation({1,2,3});
  return 0;
}
