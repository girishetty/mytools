/**
 * Given an integer array nums of unique elements, return all possible subsets (the power set).
 *
 * The solution set must not contain duplicate subsets. Return the solution in any order.
 */

#include <vector>
#include <string>
#include <iostream>

using namespace std;

void Combination(const vector<int>& nums,
                 vector<int>& comb, size_t start, size_t depth,
                 vector<vector<int>>& list) {
  if (start <= nums.size()) {
    list.push_back({comb.begin(), comb.begin() + depth});
  }

  for (size_t i = start; i < nums.size(); i++) {
    comb[depth] = nums[i];
    Combination(nums, comb, i + 1, depth + 1, list);
  }
}

/*
 * Generate all possible subsets of a given sequence of numbers
 */
vector<vector<int>> CreateCombinations(const vector<int>& nums) {
  vector<vector<int>> list;
  vector<int> comb(nums.size());
  Combination(nums, comb, 0, 0, list);
  return list;
}

void displayCombination(const string& combination);

void Combination(const string& set, string& combination, size_t start, size_t depth) {
  if (start <= set.size()) {
    //We got a combination, so print it
    combination[depth] = '\0';
    displayCombination(combination);
  }
  for (size_t index = start; index < set.size(); index++) {
    combination[depth] = set[index];
    Combination(set, combination, index + 1, depth + 1);
  }
}

/*
 * Create all possible combination (subsets of any size) for a given string
 */
void CreateCombinations(const string& set) {
  string combination(set.size(), '\0');
  Combination(set, combination, 0, 0);
}

void CreateCombinationsOfSize(const string& set, size_t combSize, size_t offset, string& combination) {
  if (combSize == 0) {
    // we have found a combination of desired size
    displayCombination(combination);
    return;
  }

  for (size_t i = offset; i <= set.size() - combSize; i++) {
    // Add this element
    combination.push_back(set[i]);
    // Create combination with rest of the element of size one less
    CreateCombinationsOfSize(set, combSize - 1, i + 1, combination);
    // Remove this element and try again with the next element
    combination.pop_back();
  }
}

/*
 * Create all possible combination (subsets) of desirable size for a given string
 */
void CreateCombinationsOfSize(const string& set, size_t size) {
  string combination;
  CreateCombinationsOfSize(set, size, 0, combination);
}

void displayCombination(const string& combination) {
  cout << "[ ";
  for (auto item : combination) {
    cout << item << " ";
  }
  cout << "]" << endl;
}

void display(const vector<int>& nums) {
  std::cout << "{ ";
  for (auto item : nums) {
    std::cout << item << " ";
  }
  std::cout << "}" << std::endl;
}

void testCombination(const vector<int>& nums) {
  auto list = CreateCombinations(nums);
  cout << "All subsets of of ";
  display(nums);

  for (const auto& item : list) {
    display(item);
  }
}

int main() {
  string set("ABC");
  std::cout << "All Subsets of " << set << " are: " << std::endl;
  CreateCombinations(set);
  std::cout << "======================================" << std::endl;

  string input("ABCDEF");
  size_t size = 3;
  std::cout << "All Subsets of " << input << " of size: " << size << " are: " << std::endl;
  CreateCombinationsOfSize(input, size);
  std::cout << "======================================" << std::endl;

  testCombination({1, 2, 3});
  return 0;
}
