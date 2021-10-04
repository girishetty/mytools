/*
 * Given a 2-D Matrix with all elements as 0s and 1s,
 * find the largest square of 1s
 */
#include <iostream>
#include <algorithm>

template <size_t rows, size_t cols>
void displayMatrix(const int (&matrix)[rows][cols]) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

template <size_t rows, size_t cols>
size_t findLargestSquare(const int (&matrix)[rows][cols]) {
  int dp[rows + 1][cols + 1] = {0};

  // Fill the 2nd row and colums of dp with first row and first column of matrix
  for (size_t i = 0; i < cols; i++) {
    dp[1][i + 1] = matrix[0][i];
  }
  for (size_t i = 0; i < rows; i++) {
    dp[i + 1][1] = matrix[i][0];
  }

  //displayMatrix(dp);
  // Start filling in the dp now
  for (size_t i = 1; i < rows; i++) {
    for (size_t j = 1; j < cols; j++) {
      if (matrix[i][j] == 1 && matrix[i - 1][j] == 1 && matrix[i][j - 1] == 1) {
        dp[i + 1] [j + 1] = dp[i][j] + 1;
      } else {
        dp[i + 1] [j + 1] = std::max({dp[i][j], dp[i + 1][j], dp[i][j + 1]});
      }
    }
  }

  return dp[rows][cols];
}

template <size_t rows, size_t cols>
void testFindLargestSquare(const int (&matrix)[rows][cols]) {
  displayMatrix(matrix);
  auto squareSize = findLargestSquare(matrix);
  std::cout << "Largest Square Size is: " << squareSize << std::endl;
}

int main() {
  int matrix_1[5][5] = { {1, 1, 0, 1, 0}, {0, 1, 1, 1, 0}, {1, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 1, 1, 0} };
  int matrix_2[5][5] = { {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1} };
  int matrix_3[5][5] = { {0, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, {0, 1, 1, 1, 0} };
  int matrix_4[5][5] = { {0, 1, 1, 1, 1}, {0, 1, 1, 1, 1}, {0, 1, 1, 1, 1}, {0, 1, 1, 1, 1}, {0, 1, 1, 1, 1} };
  int matrix_5[5][5] = {0};
  int matrix_6[5][5] = {1};

  testFindLargestSquare(matrix_1);
  testFindLargestSquare(matrix_2);
  testFindLargestSquare(matrix_3);
  testFindLargestSquare(matrix_4);
  testFindLargestSquare(matrix_5);
  testFindLargestSquare(matrix_6);

  return 0;
}
