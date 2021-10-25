/*
 * https://leetcode.com/problems/minimum-window-substring/
 *
 * Given two strings s and t of lengths m and n respectively, return the minimum window substring
 * of s such that every character in t (including duplicates) is included in the window.
 * If there is no such substring, return the empty string "".
 *
 * The testcases will be generated such that the answer is unique.
 * 
 * A substring is a contiguous sequence of characters within the string.
 *
 * Example:
 * Input: s = "ADOBECODEBANC", t = "ABC"
 * Output: "BANC"
 * Explanation: The minimum window substring "BANC" includes 'A', 'B', and 'C' from string t.
 */

#include <string>
#include <vector>
#include <iostream>

void displayVector(char name, const std::vector<size_t>& array) {
  std::cout << name << " List: [ ";
  for (auto item : array) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

std::string minWindow(const std::string& s, const std::string& t) {
  if (t.size() > s.size()) {
    return "";
  }

  std::vector<std::vector<size_t>> sMap(256, std::vector<size_t>());

  for (size_t index = 0; index < s.size(); index++) {
    sMap[s[index]].push_back(index);
  }

  int beginIndex = s.size();
  int endIndex = -1;
  for (size_t index = 0; index < t.size(); index++) {
    displayVector(t[index], sMap[t[index]]);
    if (sMap[t[index]].empty()) {
      return "";
    }
    int currIndex = sMap[t[index]].back();
    std::cout << "sMap[" << t[index] << "] size: " << sMap[t[index]].size() <<
                 ": Back: " << currIndex << ": beginIndex: " << beginIndex <<
                 ": endIndex: " << endIndex << std::endl;
    sMap[t[index]].pop_back();
    if (beginIndex > currIndex) {
      beginIndex = currIndex;
    }
    if (endIndex < currIndex) {
      endIndex = currIndex;
    }
  }

  if (beginIndex <= endIndex) {
    return s.substr(beginIndex, endIndex - beginIndex + 1);
  }

  return "";
}

void testMinWindow(std::string s, std::string t) {
  std::cout << "S: " << s << std::endl;
  std::cout << "T: " << t << std::endl;
  std::cout << "minWindow: " << minWindow(s, t) << std::endl;
}

int main() {
  testMinWindow("ADOBECODEBANC", "ABC");
  testMinWindow("A", "A");
  testMinWindow("cabwefgewcwaefgcf", "cae");

  return 0;
}
