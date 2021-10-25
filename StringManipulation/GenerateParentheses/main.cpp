/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/109/backtracking/794/
 *
 * Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
 *
 * Example 1:
 *    Input: n = 3
 *    Output: ["((()))","(()())","(())()","()(())","()()()"]
 *
 * Example 2:
 *    Input: n = 1
 *    Output: ["()"]
 *
 */

#include <vector>
#include <string>
#include <iostream>

void makeWellFormedParentheses(int openCount, int closeCount, std::string exp, std::vector<std::string>& list) {
  if (openCount == 0 && closeCount == 0) {
    list.push_back(exp);
    return;
  }

  if (openCount == 0 && closeCount > 0) {
    // We can only add remaning ')' now
    exp.push_back(')');
    makeWellFormedParentheses(openCount, closeCount - 1, exp, list);
  } else if (openCount > 0 && closeCount > 0 && openCount <= closeCount) {
    // Add '(' now
    exp.push_back('(');
    makeWellFormedParentheses(openCount - 1, closeCount, exp, list);
    exp.pop_back();
    // Alternatively, we can add ')' now
    exp.push_back(')');
    makeWellFormedParentheses(openCount, closeCount - 1, exp, list);
  }
}

std::vector<std::string> generateParenthesis(int n) {
  std::vector<std::string> list;
  std::string exp;

  makeWellFormedParentheses(n, n, exp, list);
  return list;
}

void display(const char* what, const std::vector<std::string>& list) {
  std::cout << what << " is:" << std::endl;
  for (const auto& item : list) {
    std::cout << item << std::endl;
  }
}

void testGenerateParenthesis(int n) {
  auto list = generateParenthesis(n);
  std::cout << "Input : " << n << std::endl;
  display("all combinations of well-formed parentheses", list);
}

int main() {
  testGenerateParenthesis(3);
  testGenerateParenthesis(1);
  testGenerateParenthesis(5);

  return 0;
}
