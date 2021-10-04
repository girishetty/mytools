/*
 * https://leetcode.com/problems/valid-parentheses/
 *
 * Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
 *
 * An input string is valid if:
 * 
 * Open brackets must be closed by the same type of brackets.
 * Open brackets must be closed in the correct order.
 *
 * Example:
 * #1: Input: s = "()[]{}"
 *     Output: true
 *
 * #2: Input: s = "([)]"
 *     Output: false
 */

#include <set>
#include <stack>
#include <string>
#include <iostream>
#include <unordered_map>

#define USE_SET

std::set<char> open_paranthesis_set = { '{', '[', '(' };

inline bool isOpenParantheses(char ch) {
  // Ideally we can have a map of all valid parantheses.
  // But for simplicity, this will work better
#ifdef USE_SET
  auto found = open_paranthesis_set.find(ch);
  return found != open_paranthesis_set.end();
#else
  switch (ch) {
  case '(':
  case '[':
  case '{':
    return true;
  default:
    return false;
  }

  return false;
#endif
}

std::unordered_map<char, char> close_paranthesis_set = { {'}','{'}, {']','['}, {')','('} };

inline bool isCloseParantheses(char ch, char& open) {
  // Ideally we can have a map of all valid parantheses.
  // But for simplicity, this will work better
#ifdef USE_SET
  auto found = close_paranthesis_set.find(ch);
  if (found != close_paranthesis_set.end()) {
    open = found->second;
    return true;
  }
  return false;
#else
  switch (ch) {
  case ')':
    open = '(';
    return true;
  case ']':
    open = '[';
    return true;
  case '}':
    open = '{';
    return true;
  default:
    return false;
  }

  return false;
#endif
}

bool isBalanced(const std::string& input) {
  std::stack<char> s;
  for (auto item : input) {
    if (isOpenParantheses(item)) {
      s.push(item);
      continue;
    }
    char openParentheses;
    if (isCloseParantheses(item, openParentheses)) {
      if (s.empty()) {
        return false;
      }
      auto last = s.top();
      if (last != openParentheses) {
        //if the top of stack is not the corresponding open parantheses, then we don't have matching parenthesis.
        return false;
      }
      // pop the matching one.
      s.pop();
    }
  }
  
  return (s.empty());
}

void testBalanceParentheses(std::string exp) {
  auto success = isBalanced(exp);
  std::cout << "Input: " << exp << " is" << (success ? " " : " not ") << "Balanced" << std::endl;
}

int main() {
  testBalanceParentheses("a(b)c");
  testBalanceParentheses("a(b)c)");
  testBalanceParentheses("a(bc(bc)(c)ab(cad");
  testBalanceParentheses("a()()bccab(()cad");
  testBalanceParentheses("(a)(a)((a)(a)))");
  testBalanceParentheses("(a)(a)((a)(a))");

  return 0;
}
