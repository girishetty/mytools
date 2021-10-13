/* Given N integers, check if you can reach a target integer K (e.g., 24) using those numbers and 4 binary operations.
 *
 * You can use each number once.
 * You have to use ALL numbers, including the intermediate results.
 * 4 binary operations: +, -, *, /
 * All inputs are integers. All intermediate results have to be integers as well.
 * Check if there exists a path or not (return true or false). We're not interested in the path itself.
 *
 * Examples!
 * A # [5, 3, 2, 8] K24, one solution is 5+3=8, 8x2=16, 16+8=24, return true
 * A # [5, 1, 9, 13] K24, one solution is 5+1=6, 13-9= 4, 4x6=24, return true
 * A # [1, 1, 1, 1] K24, return false
 */

#include <vector>
#include <iostream>

typedef std::function<int(const int&, const int&)> operation;

int add(const int& lhs, const int& rhs) {
  return lhs + rhs;
}

int subtract(const int& lhs, const int& rhs) {
  return lhs - rhs;
}

int multiply(const int& lhs, const int& rhs) {
  return lhs * rhs;
}

int devide(const int& lhs, const int& rhs) {
  double res = lhs / rhs;
  double floorRes = std::floor(res);
  if (floorRes == res) {
    return static_cast<int>(floorRes);
  }
  return 0;
}

// TODO: Complete the implementation
int doOperation(int lhs, int rhsIndex, char* operation, int op_index) {
  if (nums.size() == 1) {
    if (nums[0] == K) {
      return true;
    }
    return false;
  }

  result = doOperation
}

// TODO: Complete the implementation
bool isValuatesToTarger(const std::vector<int>& nums, int K) {
  if (nums.empty()) {
    return false;
  }
  if (nums.size() == 1) {
    if (nums[0] == K) {
      return true;
    }
    return false;
  }

  int result = nums[0];
  for (int i = 0; i < nums.size() - 1; i++) {
    auto lhs = nums[i];
    for (int j = i + 1; j < nums.size(); j++) {
      return doOperation(K, i, j, add) ||
             doOperation(K, i, j, subtract) ||
             doOperation(K, i, j, multiply) ||
             doOperation(K, i, j, devide);
    }
  }

 return (result == K);
}

void testIsValuatesToTarger(const std::vector<int>& nums, int K) {
  bool possible = isValuatesToTarger(nums, K);
  display("Sequence of Numbers", nums);
  if (possible) {
    std::cout << ": Evaluates to: " << K << std::endl;
  } else {
    std::cout << ": Doesn't Evaluate to: " << K << std::endl;
  }
}

int main() {
  testIsValuatesToTarger({5, 3, 2, 8}, 24); // one solution is 5+3=8, 8x2=16, 16+8=24, return true
  testIsValuatesToTarger({5, 1, 9, 13}, 24); // one solution is 5+1=6, 13-9= 4, 4x6=24, return true
  testIsValuatesToTarger({1, 1, 1, 1}, 24); // returns false

  return 0;
}
