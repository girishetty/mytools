#include <vector>
#include <string>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::string;

/**
 * calculate the result of an expression that has numbers and operatons as + and * only.
 */
long long calculate(const std::string& expression) {
  bool   validExpression = false;
  size_t expressionSize = expression.size();
  size_t index = 0;
  string operators;
  std::vector<long long> operands;
  
  for (index = 0; index < expressionSize;) {
    // find operand first.
    auto found = expression.find_first_of("*+", index);
    if (found == string::npos) {
      // end of a valid expression.
      found = expressionSize;
      validExpression = true;
    }
    auto operand = std::stoll(expression.substr(index, (found - index)));
    operands.push_back(operand);
    if (found < expressionSize) {
      // Get the corresponding operator now.
      operators.push_back(expression[found]);
    }
    index = found + 1;
  }

  if (!validExpression) {
    // Not a valid expression.
    return -1;
  }

  // Since multiplication has higher precedance over addition - do that first.
  index = 0;
  for (; true;) {
    auto found = operators.find_first_of("*", index);
    if (found == string::npos) {
      break;
    }
    auto result = (operands[found] * operands[found + 1]);

    // Save the result and remove the consumed operands and the operator.
    operands[found] = result;
    operands.erase(operands.begin() + found + 1);
    operators.erase(found, 1);
    index = found;
  }

  // Do addition operatons now.
  index = 0;
  for (; true;) {
    auto found = operators.find_first_of("+", index);
    if (found == string::npos) {
      break;
    }
    auto result = (operands[found] + operands[found + 1]);

    // Save the result and remove the consumed operands and the operator.
    operands[found] = result;
    operands.erase(operands.begin() + found + 1);
    operators.erase(found, 1);
    index = found;
  }

  // operators should have one element by now.
  // You can assert if its not the case - in case of invalid expressions
  if (operands.size() == 1) {
    return operands[0];
  }
  return -1;
}

int main() {
  vector<pair<string, long long>> expressions = {
             {"1+1+1*8+1",     11},
             {"1*2*3+5+6*3",   29},
             {"1+1+1+1+1",     5},
             {"2*2*2*2*2*2",   64},
             {"1+2+3+",        -1}, //Invalid
             };

  for (const auto& expression : expressions) {
    auto result = calculate(expression.first);
    cout << "Expression: " << expression.first << " = " << result;
    if (result == expression.second) {
      cout << "  ==> SUCCESS" << endl;
    } else {
      cout << "  ==> FAILED" << endl;
    }
  }

  return 0;
}
