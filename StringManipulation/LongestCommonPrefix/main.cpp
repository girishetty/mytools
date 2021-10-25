/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/127/strings/887/
 *
 * Write a function to find the longest common prefix string amongst an array of strings.
 *
 * If there is no common prefix, return an empty string "".
 *
 * Example 1:
 *  Input: strs = ["flower","flow","flight"]
 *  Output: "fl"
 *
 * Example 2:
 *  Input: strs = ["dog","racecar","car"]
 *  Output: ""
 *  Explanation: There is no common prefix among the input strings.
 */

#include <string>
#include <vector>
#include <iostream>

std::string longestCommonPrefix(const std::vector<std::string>& strs) {
  int size = strs.size();
  if (size == 1) {
    return strs[0];
  }

  int beginIndex = 0;
  for (; beginIndex < strs[0].size(); beginIndex++) {
    int i = 1;
    for (; i < size; i++) {
      if (beginIndex >= strs[i].size() || strs[i][beginIndex] != strs[0][beginIndex]) {
        break;
      }
    }
    if (i != size) {
      break;
    }
  }

  return strs[0].substr(0, beginIndex);
}

void display(const char* what, const std::vector<std::string>& strs) {
  std::cout << what << " is: {" << std::endl;
  for (const auto& item : strs) {
    std::cout << item << std::endl;
  }
  std::cout << "}" << std::endl;
}

void testLongestCommonPrefix(const std::vector<std::string>& strs) {
  display("Input Strings", strs);
  auto common = longestCommonPrefix(strs);
  std::cout << "Common Prefix: " << common << std::endl;
}

int main() {
  testLongestCommonPrefix({"flower", "flow", "flight"});
  testLongestCommonPrefix({"dog", "racecar", "car"});

  return 0;
}
