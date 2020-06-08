/**
 * https://www.hackerrank.com/challenges/strange-code/problem
 */

#include <vector>
#include <string>
#include <iostream>

using namespace std;

long strangeCounter(long t) {
  long initCount = 3;
  long elapsedTime = 0;
  long cycleCount = 1;

  while (t > elapsedTime) {
    auto cycleLen = initCount;
    elapsedTime += cycleLen;
    if (t <= elapsedTime) {
      break;
    }
    initCount *= 2;
  }

  return (elapsedTime - t + 1);
}

int main() {
  long t;
  cin >> t;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  long result = strangeCounter(t);

  cout << result << "\n";

  return 0;
}

