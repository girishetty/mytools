/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/110/sorting-and-searching/806/
 *
 * Write an efficient algorithm that searches for a target value in an m x n integer matrix.
 * The matrix has the following properties:
 *  - Integers in each row are sorted in ascending from left to right.
 *  - Integers in each column are sorted in ascending from top to bottom.
 *
 *   1   4   7  11  15
 *   2   5   8  12  19
 *   3   6   9  16  22
 *  10  13  14  17  24
 *  18  21  23  26  30
 *
 * Example 1:
 *    Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 5
 *    Output: true
 *
 * Example 2:
 *    Input: matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 20
 *    Output: false
 *
 */

#include <vector>
#include <iomanip>
#include <iostream>

void display(const char* what, const std::vector<std::vector<int>>& matrix) {
  std::cout << what << ": {" << std::endl;
  for (const auto& rows : matrix) {
    std::cout << "[ ";
    for (auto item : rows) {
      std::cout << std::setw(3) << item << " ";
    }
    std::cout << "]," << std::endl;
  }
  std::cout << "}" << std::endl;
}

std::vector<int> searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
  int rows = matrix.size();
  int cols = matrix[0].size();

  if (target < matrix[0][0] || target > matrix[rows - 1][cols - 1]) {
    return {-1, -1};
  }

  // Start from the top right (first row, last element)
  // and then go down the column, if the number is more
  // OR go to previous row, if the number is less
  for (int row = 0, col = cols - 1; row < rows && col >= 0; ) {
    int diff = target - matrix[row][col];
    if (diff == 0) {
      return {row, col};
    }
    if (diff > 0) {
      // Not in this row, so try next row
      row++;
    } else {
      // Not in this column, so try previous col
      col--;
    }
  }

  return {-1, -1};
}

void testSearchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
  display("Input", matrix);
  auto index = searchMatrix(matrix, target);

  std::cout << target << " is found at the index: ["
            << index[0] << ", " << index[1] << "]" << std::endl;
}

int main() {
  std::vector<std::vector<int>> matrix ({{ 1,  4,  7, 11, 15},
                                         { 2,  5,  8, 12, 19},
                                         { 3,  6,  9, 16, 22},
                                         {10, 13, 14, 17, 24},
                                         {18, 21, 23, 26, 30}});
  testSearchMatrix(matrix, 5);
  testSearchMatrix(matrix, 20);
  testSearchMatrix(matrix, 15);

  return 0;
}
