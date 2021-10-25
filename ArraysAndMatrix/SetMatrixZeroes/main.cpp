/*
 * https://leetcode.com/problems/set-matrix-zeroes/
 *
 * Given an m x n integer matrix matrix, if an element is 0, set its entire row and column to 0's, and return the matrix.
 * You must do it in place.
 *
 * Example:
 *    Input: matrix = [[0,1,2,0],[3,4,5,2],[1,3,1,5]]
 *    Output: [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
 *
 */

#include <set>
#include <vector>
#include <iostream>

void display(const char* what, const std::vector<std::vector<int>>& matrix) {
  std::cout << what << " is:" << std::endl;
  for (const auto& row : matrix) {
    for (auto item : row) {
      std::cout << item << "  ";
    }
    std::cout << std::endl;
  }
}

void setZeroes(std::vector<std::vector<int>>& matrix) {
  int rows = matrix.size();
  int cols = matrix[0].size();
  std::set<int> zeroRows;
  std::set<int> zeroCols;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      if (matrix[row][col] == 0) {
        zeroRows.insert(row);
        zeroCols.insert(col);
      }
    }
  }

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      if (zeroRows.find(row) != zeroRows.end() || zeroCols.find(col)!= zeroCols.end() ) {
          matrix[row][col] = 0;
      }
    }
  }
}

void testSetZeroes(std::vector<std::vector<int>>& matrix) {
  display("Input Matrix", matrix);
  setZeroes(matrix);
  display("Zero'd Matrix", matrix);
}

int main() {
  std::vector<std::vector<int>> matrix = {{0,1,2,0},{3,4,5,2},{1,3,1,5}};
  testSetZeroes(matrix);

  return 0;
}
