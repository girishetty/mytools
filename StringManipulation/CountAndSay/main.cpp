/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/127/strings/886/
 *
 * The count-and-say sequence is a sequence of digit strings defined by the recursive formula:
 *
 *   countAndSay(1) = "1"
 *   countAndSay(n) is the way you would "say" the digit string from countAndSay(n-1),
 *                  which is then converted into a different digit string.
 * To determine how you "say" a digit string, split it into the minimal number of groups so that
 * each group is a contiguous section all of the same character. Then for each group, say the
 * number of characters, then say the character. To convert the saying into a digit string,
 * replace the counts with a number and concatenate every saying.
 *
 * For example, the saying and conversion for digit string "3322251":
 *   two 3's, three 2's, one 5, one 1
 *   2 3 + 3 2 + 1 5 + 1 1
 *   23321511
 *
 * Example#1:
 *   Input: n = 4
 *   Output: "1211"
 *   Explanation:
 *     countAndSay(1) = "1"
 *     countAndSay(2) = say "1" = one 1 = "11"
 *     countAndSay(3) = say "11" = two 1's = "21"
 *     countAndSay(4) = say "21" = one 2 + one 1 = "12" + "11" = "1211"
 *
 * Example#2:
 *   Input: n = 5
 *   Output: "111221"
 *   Explanation:
 *     countAndSay(1) = "1"
 *     countAndSay(2) = say "1" = one 1 = "11"
 *     countAndSay(3) = say "11" = two 1's = "21"
 *     countAndSay(4) = say "21" = one 2 + one 1 = "12" + "11" = "1211"
 *     countAndSay(5) = say "1211" = one 1 + one 2 + two 1 = "12" + "11" = "111221"
 */

#include <string>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, std::string> memo;

std::string getCountString(const std::string& res) {
  std::string result;
  int size = res.size();

  for (int i = 0; i < size;) {
    int j = i + 1;
    for (; j < size && res[j] == res[i]; j++);
    int count = j - i;
    result += std::to_string(count) + res[i];
    i = j;
  }

  return result;
}
    
std::string countAndSay(int n) {
  auto found = memo.find(n);
  if (found != memo.end()) {
    return found->second;
  }
  
  // base case
  if (n == 1) {
    memo[1] = "1";
    return "1";
  }

  auto res = countAndSay(n-1);
  memo[n] = getCountString(res);
  return memo[n];
}

int main() {
  int n = 5;
  auto result = countAndSay(n);
  std::cout << "N is: " << n << " Count And Say: " << result << std::endl;

  n = 9;
  result = countAndSay(n);
  std::cout << "N is: " << n << " Count And Say: " << result << std::endl;

  n = 8;
  result = countAndSay(n);
  std::cout << "N is: " << n << " Count And Say: " << result << std::endl;

  n = 3;
  result = countAndSay(n);
  std::cout << "N is: " << n << " Count And Say: " << result << std::endl;

  return 0;
}
