/*
 * https://leetcode.com/problems/longest-substring-without-repeating-characters/
 * Given a string s, find the length of the longest substring without repeating characters.
 */

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

int lengthOfLongestSubstringUsingVector(const std::string& s) {
  size_t max_length = 0;
  size_t start = 0;
  std::vector<uint8_t> repeating(256, 0);
  
  for (size_t i = 0; i < s.size(); i++) {
    if (repeating[s[i]] > 0) {
      // Element is repeating. This would be end of this substring.
      max_length = std::max(max_length, i - start);
      // Now reset all elements in repeating, until we find this element
      for (; s[start] != s[i]; start++) {
        repeating[s[start]] = false;
      }
      // So our next substring starts from the next element afert this repeated element.
      start++;
    } else {
      repeating[s[i]] = 1;
    }
  }

  return std::max(max_length, s.size() - start);
}

int lengthOfLongestSubstringUsingMap(const std::string& s) {
  size_t max_length = 0;
  size_t start = 0;
  std::unordered_map<char, size_t> positionMap;

  for (size_t i = 0; i < s.size(); i++) {
    auto found = positionMap.find(s[i]);
    if (found != positionMap.end()) {
      // Element is repeating. This would be end of this substring.
      max_length = std::max(max_length, i - start);
      // Now remove all the elements in the positionMap for s[start] till first occurance of this elememt
      size_t end = found->second;
      // Update the current position
      found->second = i;
      for (; start < end; start++) {
        positionMap.erase(s[start]);
      }
      start++;
    } else {
      positionMap[s[i]] = i;
    }
  }

  return std::max(max_length, s.size() - start);
}

int main() {
  std::string s1("abcabcbb");
  std::string s2("bbbbb");
  std::string s3("pwwkew");
  std::string s4("");
  std::string s5("abcabcdbb");
  std::string s6("dvdf");
  std::string s7("umvejcuuk");
  std::cout << "Size of Longest Substring [" << s1 << "] is : " << lengthOfLongestSubstringUsingVector(s1) << ": " << lengthOfLongestSubstringUsingMap(s1) << std::endl;
  std::cout << "Size of Longest Substring [" << s2 << "] is : " << lengthOfLongestSubstringUsingVector(s2) << ": " << lengthOfLongestSubstringUsingMap(s2) << std::endl;
  std::cout << "Size of Longest Substring [" << s3 << "] is : " << lengthOfLongestSubstringUsingVector(s3) << ": " << lengthOfLongestSubstringUsingMap(s3) << std::endl;
  std::cout << "Size of Longest Substring [" << s4 << "] is : " << lengthOfLongestSubstringUsingVector(s4) << ": " << lengthOfLongestSubstringUsingMap(s4) << std::endl;
  std::cout << "Size of Longest Substring [" << s5 << "] is : " << lengthOfLongestSubstringUsingVector(s5) << ": " << lengthOfLongestSubstringUsingMap(s5) << std::endl;
  std::cout << "Size of Longest Substring [" << s6 << "] is : " << lengthOfLongestSubstringUsingVector(s6) << ": " << lengthOfLongestSubstringUsingMap(s6) << std::endl;
  std::cout << "Size of Longest Substring [" << s7 << "] is : " << lengthOfLongestSubstringUsingVector(s7) << ": " << lengthOfLongestSubstringUsingMap(s7) << std::endl;

  return 0;
}
