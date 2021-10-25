/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/96/sorting-and-searching/774/
 *
 * You are a product manager and currently leading a team to develop a new product.
 * Unfortunately, the latest version of your product fails the quality check.
 * Since each version is developed based on the previous version,
 * all the versions after a bad version are also bad.
 *
 * Suppose you have n versions [1, 2, ..., n] and you want to find out the first bad one,
 * which causes all the following ones to be bad.
 * 
 * You are given an API bool isBadVersion(version) which returns whether version is bad.
 * Implement a function to find the first bad version. You should minimize the number of calls to the API.
 *
 */

#include <cstdlib>
#include <iostream>

/*
 * BEGIN Framework
 * ==================================================================================================================
 * Assume that this has been provided by the framework.
 */

int gBadVersion = 0;

void setBadVersion(int max) {
  srand(time(0));
  int rand_num = std::rand();
  gBadVersion = rand_num % (max + 1);
}

bool isBadVersion(int version) {
  return (version >= gBadVersion);
}
/*
 * END Framework
 * ==================================================================================================================
 */

int firstBadVersion(int n) {
  int begin = 1;
  int end = n;

  while (begin < end) {
    long sum = static_cast<long>(begin) + end;
    int middle = sum >> 1;
    if (isBadVersion(middle)) {
      if (middle == 0 || !isBadVersion(middle - 1)) {
        return middle;
      }
      end = middle - 1;
    } else {
      begin = middle + 1;
    }
  }

  return begin;
}

void testBadVersion(int n) {
  setBadVersion(n);
  int version = firstBadVersion(n);
  std::cout << "Current Version: " << n << " First Bad Version: " << version << std::endl;
}

int main() {
  testBadVersion(5);
  testBadVersion(100);
  return 0;
}
