/**
 * Fibonacci using dynamic programming.
 * https://www.youtube.com/watch?v=EEb6JP3NXBI
 */

#include <iomanip>
#include <iostream>
#include "FibonacciGenerator.h"

using std::cout;
using std::endl;
using std::endl;
using std::setw;
using std::vector;

static void displayMatrix(const vector<vector<unsigned long long>>& matrix) {
  for (auto list : matrix) {
    for (auto item : list) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
  }
  cout << "========================" << endl;
}

static void displayList(const vector<int>& list) {
  for (auto item : list) {
    cout << setw(4) << item << "    ";
  }
  cout << endl << "========================" << endl;
}

// This function returns all the bits that are set as list, for a given number N
static vector<int> getTwoPowerList(int n) {
  vector<int> list;
  // start from LSB.
  int bitCount = 0;
  list.clear();
  do {
    uint8_t bit = n & 0x1;
    if (bit) {
      list.push_back(1 << bitCount);
    }
    bitCount++;
    n = n >> 1;
  } while (n != 0);

  return list;
}

// multiply 2 matrices of size [2 x 2] 
// |a b| X |e f| = |ae + bg, af + bh|
// |c d|   |g h|   |ce + dg, cf + dh|
static vector<vector<unsigned long long>> multiplyMatrix(
    const vector<vector<unsigned long long>>& left,
    const vector<vector<unsigned long long>>& right) {
  vector<vector<unsigned long long>> result(2, vector<unsigned long long>(2, 0));

  result[0][0] = left[0][0] * right[0][0] + left[0][1] * right[1][0];
  result[0][1] = left[0][0] * right[0][1] + left[0][1] * right[1][1];
  result[1][0] = left[1][0] * right[0][0] + left[1][1] * right[1][0];
  result[1][1] = left[1][0] * right[0][1] + left[1][1] * right[1][1];

  return result;
}

// Raise a matrix by power ==> pow(matrix, power);
vector<vector<unsigned long long>> FibonacciGenerator::matrixToThePower(const vector<vector<unsigned long long>>& matrix, int power) {
  // Check if we have already computed it.
  auto found = mPowerMatrix.find(power);
  if (found != mPowerMatrix.end()) {
    return found->second;
  }
  if (power == 1) {
    mPowerMatrix[power] = matrix;
    return matrix;
  }
  auto result = matrixToThePower(matrix, power >> 1);
  result = multiplyMatrix(result, result);
  mPowerMatrix[power] = result;
  return result;
}

void FibonacciGenerator::raiseToThePower(const vector<vector<unsigned long long>>& matrix, int n) {
  // Breakdown given number as sum of power of 2.
  // For example 13 can be written as 1 + 4 + 8 (2 pow 0 + 2 pow 2 + 2 pow 3)
  // So n pow 13 = (n pow 1) * (n pow 4) * (n pow 8)
  auto found = mPowerList.find(n);
  if (found == mPowerList.end()) {
    mPowerList[n] = getTwoPowerList(n);
    found = mPowerList.find(n);
  }
  const auto& list = found->second;

  // For each such smaller 2 power numbers, raise the matrix to that power.
  // Note that matrixToThePower saves the power and corresponding power matrix for the future computation.
  // For example: Matrix pow 4 can be computed as (Matrix pow 2 * Matrix pow 2)
  // For example: Matrix pow 8 can be computed as (Matrix pow 4 * Matrix pow 4)
  // So we save lots of time by memoizing it.
  for (auto power : list) {
    // Since we are saving them in mPowerMatrix, we can ignore the return values.
    matrixToThePower(matrix, power);
  }

  // Now, for all the list of power of two problems, multiply them all to get the final matrix.
  // For example, if n is 13, which means we have raised the matrix to 1, 4 and 8 and saved it already
  // in above step, what we need to do now is:
  // Matrix pow 13 = Matrix pow 1 * Matrix pow 4 * Matrix pow 8
  auto firstPower = list[0];
  auto firstItem = mPowerMatrix[firstPower];
  for (int index = 1; index < list.size(); index++) {
    auto secondPower = list[index];
    auto secondItem = mPowerMatrix[secondPower];
    auto result = multiplyMatrix(firstItem, secondItem);

    // Before going for the next item in the power list, update the first matrix for the multiplication.
    firstPower += secondPower;
    firstItem = result;

    // Also save the power matrix for this result.
    mPowerMatrix[firstPower] = firstItem;
  }
}

unsigned long long FibonacciGenerator::getFibonacci(int n) {
  // Base cases.
  if (n == 0) {
    return 0;
  }
  if (n < 3) {
    return 1;
  }
  // Check if we have already computed this Nth Fibonacci.
  auto found = mPowerMatrix.find(n);
  if (found == mPowerMatrix.end()) {
    // Donald E. Knuth solution for Fibonacci number:
    // |fib(n+1)  fib(n)  | = |1 1| power n
    // |fib(n)    fib(n-1)|   |1 0|
    raiseToThePower(mBaseMatrix, n);
    found = mPowerMatrix.find(n);
  }

  return found->second[0][1];
}
