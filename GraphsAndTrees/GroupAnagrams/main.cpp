/*
 * https://leetcode.com/problems/group-anagrams/
 *
 * Given an array of strings strs, group the anagrams together. You can return the answer in any order.
 * An Anagram is a word or phrase formed by rearranging the letters of a different word or phrase, typically using all the original letters exactly once.
 *
 * Example:
 *   Input: strs = ["eat","tea","tan","ate","nat","bat"]
 *   Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
 */
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

inline std::vector<size_t> getMap(const std::string& str) {
  std::vector<size_t> currentMap(256, 0);

  for (auto item : str) {
    currentMap[item]++;
  }
  return currentMap;
}
    
std::vector<std::vector<std::string>> groupAnagramsUsingSortAndMaps(std::vector<std::string>& strs) {
  std::unordered_map<std::string, std::vector<std::string>> sortedMap;

  for (const auto& item: strs) {
    std::string sorted = item;
    std::sort(sorted.begin(), sorted.end());
    sortedMap[sorted].push_back(item);
  }

  std::vector<std::vector<std::string>> results;
  for (const auto& item : sortedMap) {
    results.push_back(item.second);
  }

  return results;
}

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
  std::vector<std::vector<std::string>> results;
  std::vector<std::vector<size_t>> allMaps;

  for (const auto& item: strs) {
    allMaps.push_back(getMap(item));
  }

  for (size_t i = 0; i < strs.size(); i++) {
    std::vector<std::string> groups;
    groups.push_back(strs[i]);
    for (size_t j = i + 1; j < strs.size(); j++) {
      if (allMaps[i] == allMaps[j]) {
        groups.push_back(strs[j]);
        strs.erase(strs.begin() + j);
        allMaps.erase(allMaps.begin() + j);
        j--;
      }
    }
    results.push_back(groups);
  }
  
  return results;
}

inline void displayVector(const std::vector<std::string>& list) {
  std::cout << "[ ";
  for (const auto& item : list) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

void testGroupAnagrams(std::vector<std::string> strs) {
  displayVector(strs);
#ifdef USE_HASH
  auto groups = groupAnagrams(strs);
#else
  auto groups = groupAnagramsUsingSortAndMaps(strs);
#endif
  for (const auto& item : groups) {
    displayVector(item);
  }
}

int main() {
  testGroupAnagrams({"eat","tea","tan","ate","nat","bat"});
  testGroupAnagrams({"","",""});
  testGroupAnagrams({"a","a","a"});

  return 0;
}

