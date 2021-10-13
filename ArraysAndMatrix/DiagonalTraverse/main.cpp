/*
 * https://leetcode.com/problems/diagonal-traverse/description/
 *
 * Given an m x n matrix mat, return an array of all the elements of the array in a diagonal order
 *
 * Example#1:
 * Input: mat = [[1,2,3],[4,5,6],[7,8,9]]
 * Output: [1,2,4,7,5,3,6,8,9]
 *
 * Example#2:
 * Input: mat = [[1,2],[3,4]]
 * Output: [1,2,3,4]
 */

#include <vector>
#include <iostream>

std::vector<int> findDiagonalOrder(const std::vector<std::vector<int>>& matrix) {
  std::vector<int> list;

  int rows = matrix.size();
  if (rows == 0) {
    return list;
  }
  int cols = matrix[0].size();
  if (cols == 0) {
    return list;
  }

  int row = 0;
  int col = 0;

  for (; row < rows && col < cols; ) {
    // go up the diagonal (towards top right)
    for (; row >= 0 && col < cols; row--, col++) {
      list.push_back(matrix[row][col]);
    }

    // Bring back row and col to the right position as we are going to change direction now
    row += 1;
    if (col == cols) {
      col--;
      row++;
    }

    // go down the diagonal (towards bottom left)
    for (; row < rows && col >= 0; row++, col--) {
      list.push_back(matrix[row][col]);
    }

    // Bring back row and col to the right position as we are going to change direction now
    col += 1;
    if (row == rows) {
      row--;
      col++;
    }
  }

  return list;
}

void display(const char* what, const std::vector<int>& list) {
  std::cout << what << " is: [ ";
  for (const auto& item : list) {
    std::cout << item << " ";
  }
  std::cout << "]" << std::endl;
}

void display(const char* what, const std::vector<std::vector<int>>& matrix) {
  std::cout << what << " is: " << std::endl;
  for (const auto& row : matrix) {
    for (const auto& item : row) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
}

void testFindDiagonalOrder(const std::vector<std::vector<int>>& matrix) {
  auto list = findDiagonalOrder(matrix);
  display("Givien Matrix", matrix);
  display("Diagonal Order", list);
}

int main() {
  testFindDiagonalOrder({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
  testFindDiagonalOrder({{1, 2},{3, 4}});
  return 0;
}

