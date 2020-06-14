/**
 * Fibonacci using dynamic programming.
 * https://www.youtube.com/watch?v=EEb6JP3NXBI
 */

#include <iostream>
#include "FibonacciGenerator.h"

using std::cin;
using std::cout;
using std::endl;

int main() {
  int n;
  cout << "Enter a number: ";
  cin >> n;

  FibonacciGenerator fb;
  cout << n << "th Fibonacci number is: " << fb.getFibonacci(n) << endl;

  return 0;
}
