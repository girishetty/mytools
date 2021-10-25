/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/109/backtracking/793/
 *
 * Letter Combinations of a Phone Number:
 * Given a string containing digits from 2-9 inclusive, return all possible letter combinations
 * that the number could represent. Return the answer in any order.
 * 
 * A mapping of digit to letters (just like on the telephone buttons) is given below.
 * Note that 1 does not map to any letters.
 *
 * Example 1:
 *     Input: digits = "23"
 *     Output: ["ad","ae","af","bd","be","bf","cd","ce","cf"]
 *
 * Example 2:
 *    Input: digits = ""
 *    Output: []
 *
 * Example 3:
 *    Input: digits = "2"
 *    Output: ["a","b","c"]
 *
 */

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <sys/time.h>

/*
 * List that maps the digits associated with digit 2 (at index 0)
 * to digit 9 (at index 7)
 */
std::vector<std::string> digitMap { "abc",   // for 2
                                    "def",   // for 3
                                    "ghi",   // for 4
                                    "jkl",   // for 5
                                    "mno",   // for 6
                                    "pqrs",  // for 7
                                    "tuv",   // for 8
                                    "wxyz"}; // for 9 

/*
 * Vector based fast solution
 */
std::vector<std::string> letterCombinationsFast(const std::vector<int>& pads) {
  std::vector<std::string> letterCombs;
  letterCombs.push_back("");

  int size = pads.size();
  for (int start = 0; start < size; start++) {
    auto interim = std::move(letterCombs);
    letterCombs.clear();
    for (const auto& item : interim) {
      for (auto letter : digitMap[pads[start]]) {
        letterCombs.push_back(item + letter);
      }
    }
  }

  return letterCombs;
}

/*
 * Queue based backtracking solution
 */
std::vector<std::string> letterCombinations(const std::vector<int>& pads) {
  std::vector<std::string> letterCombs;
  std::queue<std::string>  comboQueue;

  // Queue starts with an empty item
  comboQueue.push("");
  while (!comboQueue.empty()) {
    // Pop the first item in the queue
    auto combo = comboQueue.front();
    comboQueue.pop();

    // If we have already found one combo, add it to our result
    if (combo.size() == pads.size()) {
      letterCombs.push_back(combo);
    } else {
      // Now append all possible digits with this pad to the current combo
      for (auto letter : digitMap[pads[combo.size()]]) {
        comboQueue.push(combo + letter);
      }
    }
  }

  return letterCombs;
}

std::vector<std::string> letterCombinations(const std::string& digits, bool fast = false) {
  // Construct pads (of list of letters at given digit)
  std::vector<int> pads;
  for (const char digit : digits) {
    // digit is in the range of '2' to '9'
    int num = digit - '2';
    pads.push_back(num);
  }

  if (fast) {
    return letterCombinationsFast(pads);
  }
  return letterCombinations(pads);
}

void display(const char* what, const std::vector<std::string>& comb) {
  std::cout << what << " is: ";
  for (const auto & item : comb) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

inline uint64_t getWallClockTime() {
  struct timeval time;
  if (gettimeofday(&time, NULL)) {
    return 0;
  }
  return (uint64_t)time.tv_sec * 1000000 + (uint64_t)time.tv_usec;
}

void testLetterCombinations(const std::string& digits) {
  std::cout << "Digits: " << digits << std::endl;

  auto start = getWallClockTime();
  auto comb = letterCombinations(digits);
  auto end = getWallClockTime();
  //display("Possible Combinations", comb);
  std::cout << "Took " << end - start << " microseconds" << std::endl;

  start = getWallClockTime();
  comb = letterCombinations(digits, true);
  end = getWallClockTime();
  //display("Possible Combinations", comb);
  std::cout << "Took " << end - start << " microseconds" << std::endl;
}

int main() {
  testLetterCombinations("23");
  testLetterCombinations("7856432823");
  testLetterCombinations("3");

  return 0;
}
