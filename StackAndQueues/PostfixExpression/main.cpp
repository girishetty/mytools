/*
 * Write a function, that evaluates postfix expression containing +, -, * and /
 *
 * Input: ["2", "1", "+", "3", "*"]
 * Output: 9
 * Explanation: ((2 + 1) * 3) = 9
 */

#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

typedef std::function<double(const double&, const double&)> operation;

double add(const double& lhs, const double& rhs) {
  return lhs + rhs;
}

double subtract(const double& lhs, const double& rhs) {
  return lhs - rhs;
}

double multiply(const double& lhs, const double& rhs) {
  return lhs * rhs;
}

double devide(const double& lhs, const double& rhs) {
  return lhs / rhs;
}

std::map<const char, operation> gOpsMap { {'+', add},
                                          {'-', subtract},
                                          {'*', multiply},
                                          {'-', devide},
                                        };

bool isOperation(const char ch, operation& op) {
  auto found = gOpsMap.find(ch);
  if (found != gOpsMap.end()) {
    op = found->second;
    return true;
  }
  return false;
}

double evaluateExpression(const std::vector<std::string>& input) {
  std::stack<double> valuator;
  
  if (input.size() < 3) {
    //invalid
    std::cout << "Invalid Postfix expression!";
    return 0;
  }
  
  double result = 0;
  for (const auto& item : input) {
    operation op;
    if (isOperation(item[0], op)) {
      double rhs = valuator.top();
      valuator.pop();
      double lhs = valuator.top();
      valuator.pop();
      result = op(lhs, rhs);
      valuator.push(result);
    } else {
      valuator.push(std::stoi(item));
    }
  }
  
  if (valuator.size() != 1) {
    //invalid
    std::cout << "Invalid Postfix expression!";
    return 0;
  }
  return result;
}

void display(const char* what, const std::vector<std::string>& input) {
  std::cout << what << ": [ ";
  for (const auto& item : input) {
    std::cout << item << " ";
  }

  std::cout << " ]" << std::endl;
}

void testEvaluateExpression(const std::vector<std::string>& input) {
  display("Postfix Expression", input);
  std::cout << "Valuates to: " << evaluateExpression(input) << std::endl;
}

int main() {
  testEvaluateExpression({"2", "1", "+", "3", "*"});
  testEvaluateExpression({"20", "11", "+", "16", "*"});

  return 0;
}
