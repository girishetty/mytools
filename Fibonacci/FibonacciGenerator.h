/**
 * Fibonacci using dynamic programming.
 * https://www.youtube.com/watch?v=EEb6JP3NXBI
 */

#pragma once

#include <vector>
#include <unordered_map>

class FibonacciGenerator {
public:
  unsigned long long getFibonacci(int n);

private:
  std::vector<std::vector<unsigned long long>> matrixToThePower(const std::vector<std::vector<unsigned long long>>& matrix, int power);
  void raiseToThePower(const std::vector<std::vector<unsigned long long>>& matrix, int n);

private:
  // Map of generated Fibonacci numbers.
  std::unordered_map<int, unsigned long long> mFibList;
  // Map of representation using 2 power numbers.
  std::unordered_map<int, std::vector<int>> mPowerList;
  // Map of nth power for a give number.
  std::unordered_map<int, std::vector<std::vector<unsigned long long>>> mPowerMatrix;

  // Base case.
  // |fib2 fib1|
  // |fib1 fib0|
  const std::vector<std::vector<unsigned long long>> mBaseMatrix = {
     {1, 1},
     {1, 0} };

};
