/*
Given a string with alpha-numeric characters and parentheses, return a string with balanced parentheses by removing the fewest characters possible. You cannot add anything to the string.

examples: 
balance("()") -> "()"
balance("a(b)c)") -> "a(b)c"/ "a(bc)"
balance(")(") -> ""
balance("(((((") -> ""
balance("(()()(") -> "()()"
balance(")(())(") -> "(())"
balance(")())(()()(") -> "()()()"


()(
(()

()

until (begin < end)
s[begin] matches s[end] ==> begin++, end--
else s[begin] matches s[end-1] ==> remove(end); end--
else s[begin+1] matches s[end] ==> remove(end); begin+++
else remove(end and begin) begin++, end--

*/

#include <stack>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::stack;
using std::string;

bool isBalanced(const string& input) {
  stack<char> s;
  for (auto item : input) {
    if (isalpha(item)) {
      continue;
    }
    if (item == '(') {
      s.push(item);
      continue;
    }
    // If we are here, thats only because item is ')'
    if (s.empty()) {
      return false;
    }
    auto last = s.top();
    if (last != '(') {
      //if the top of stack is not ')', then we don't have matching parenthesis.
      return false;
    }
    // pop the matching one.
    s.pop();
  }
  
  return (s.empty());
}

void balanceParentheses(string& input) {
  size_t begin = 0;
  size_t end = input.size() - 1;
  
  for (; begin < end; ) {
    for (; isalnum(input[begin]); begin++);
    for (; isalnum(input[end]); end--);
    if (begin > end) {
      break;
    }
    if (input[begin] == '(') {
      if(input[end] == ')') {
        begin++;
        end--;
        continue;
      }
      if (input[end] == '(') {
        // unbalanced "(", remove it
        input.erase(end, 1);
        end--;
      }
    } else if (input[end] == ')') {
      // end is ")" without a matching "(" ==> delete it.
      input.erase(end, 1);
      end--;
    }
  }

  if (begin == end && !isalnum(input[begin])) {
    input.erase(begin, 1);
  }
}

int main() {
  string exp = "a(b)c)";
  cout << "Input: " << exp;
  balanceParentheses(exp);
  cout << " ==> Balanced: " << exp << endl;
  exp = "a(bc(bc)(c)ab(cad";
  cout << "Input: " << exp;
  balanceParentheses(exp);
  cout << " ==> Balanced: " << exp << endl;

  return 0;
}
