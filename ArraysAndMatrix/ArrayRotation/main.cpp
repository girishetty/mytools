/*
 * Rotate an array by a distnace towards left or right
 *
 * Example#1:
 * Input Array: {1,2,3,4,5,6,7}
 * Direction: rotate right by 3
 * Output: [5 6 7 1 2 3 4 ]
 *
 * Example#2:
 * Input Array: {1,2,3,4,5,6,7}
 * Direction: rotate left by 3
 * Output: {4 5 6 7 1 2 3 }
 *
 */
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void display(const char* what, const vector<int> nums) {
  std::cout << what << " is: [";
  for (auto item : nums) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

void rotateRight(vector<int> nums, int k) {
  int size = nums.size();
  k = k % size;

  display("Orig", nums);
  if (k > 0) {
    auto moveBegin = nums.begin() + (size - k);
    nums.insert(nums.begin(), moveBegin, nums.end());
    nums.resize(size);
  }
  display("Right Rotated", nums);
}

// Complete the rotateLeft function below.
vector<int> rotateLeft(const vector<int>& a, int d) {
  display("Orig", a);
  size_t arraySize = a.size();
  vector<int> leftRotated(arraySize);
  int beginIndex = (arraySize + d) % arraySize;
  int toCopySize = arraySize - beginIndex;
  std::copy(a.begin() + beginIndex, a.begin() + beginIndex + toCopySize, leftRotated.begin());
  if (beginIndex > 0) {
    std::copy(a.begin(), a.begin() + beginIndex, leftRotated.begin() + toCopySize);
  }

  display("Left Rotated", leftRotated);
  return leftRotated;
}

int main() {
  rotateRight({1,2,3}, 1);
  rotateRight({1,2,3}, 2);
  rotateRight({1,2,3}, 3);
  rotateRight({1,2,3}, 4);
  rotateRight({1,2,3,4,5,6,7}, 3);
  rotateLeft({1,2,3,4,5,6,7}, 3);

  return 0;
}
