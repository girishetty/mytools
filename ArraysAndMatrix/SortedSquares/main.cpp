/*
 * Implement: Given sorted numbers, generate sorted list of squares of those numbers
 */

#include <vector>
#include <iostream>

void display(const char* what, const std::vector<int>& array) {
  std::cout << what << " is: [ ";

  for (const auto& item : array) {
    std::cout << item << " ";
  }

  std::cout << "]" << std::endl;
}

std::vector<int> getSortedSquares(const std::vector<int>& array) {
  std::vector<int> squares(array.size());
  int left = 0;
  int right = array.size() - 1;

  for (int index = right; index >= 0; index--) {
    auto item = array[right];
    if (std::abs(item) < std::abs(array[left])) {
      item = array[left];
      left++;
    } else {
      right--;
    }
    squares[index] = item * item;
  }

  return squares;
}

void testGetSortedSquares(const std::vector<int>& numbers) {
  auto squares = getSortedSquares(numbers);

  display("Given Sorted Array", numbers);
  display("Sorted Square Array", squares);
}

int main() {
  testGetSortedSquares({ -5, -4, -2, 1, 4, 6, 7, 9});
  testGetSortedSquares({ -9, -7, -6, -5, -4, -2, -1, 4});
  testGetSortedSquares({ -9, -7, -6, -5, -4, -2, -1});

  return 0;
}
