// # Determine if a matrix is Toeplitz (diagonal constant) or not.
// #
// # Example:
// #
// #      4 5 6 7 6
// #      9 4 5 6 7
// #      8 9 4 5 6
// #      3 8 9 4 5

#include <vector>
#include <iostream>

bool isToeplitzDiagnol(const std::vector<std::vector<int>>& matrix, int row, int col) {
  for (; row < matrix.size() && col < matrix[0].size(); row++, col++) {
    if (matrix[row-1][col-1] != matrix[row][col]) {
      return false;
    }
  }
  
  return true;
}

bool isToeplitz(const std::vector<std::vector<int>>& matrix) {
  if (matrix.empty() || matrix[0].empty()) {
    return false;
  }
  
  int row = 1;
  for (; row < matrix.size(); row++) {
    if (!isToeplitzDiagnol(matrix, row, 1)) {
      return false;
    }
  }

  int col = 2;
  for (; col < matrix[0].size(); col++) {
    if (!isToeplitzDiagnol(matrix, 1, col)) {
      return false;
    }
  }

  return true;
}

void display(const char* what, const std::vector<std::vector<int>>& matrix) {
  std::cout << what << std::endl;

  for (const auto& row : matrix) {
    for (const auto& item : row) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
}

void testIsToeplitz(const std::vector<std::vector<int>>& matrix) {
  display("Input Matrix", matrix);
  if (isToeplitz(matrix)) {
    std::cout << "Is a Toeplitz" << std::endl;
  } else {
    std::cout << "Is not a Toeplitz" << std::endl;
  }
}

int main() {
  testIsToeplitz({ {4, 5, 6, 7, 6},
                   {9, 4, 5, 6, 7},
                   {8, 9, 4, 5, 6},
                   {3, 8, 9, 4, 5} });

  testIsToeplitz({ {4, 5, 6, 7, 6},
                   {9, 4, 5, 6, 7},
                   {7, 9, 4, 5, 6},
                   {3, 8, 9, 4, 5} });

  return 0;
}
