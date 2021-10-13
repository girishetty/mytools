/**
 * https://www.hackerrank.com/challenges/matrix-rotation-algo/problem
 */

/**
  Sample input:
5 4 7
1 2 3 4
7 8 9 10
13 14 15 16
19 20 21 22
25 26 27 28
*/

#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace std;

void printMatrix(const char* what, const vector<vector<int>>& matrix) {
  std::cout << what << " is:" << std::endl;
  for (const auto& row : matrix) {
    for (auto item : row) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
  }
}

void rotateAntiClockwiseBy90Degree(vector<vector<int>>& matrix) {
  printMatrix("Original Matrix", matrix);

  int matrixSize = matrix.size();
  // The first cycle is formed by its first row, last column, last row, and first column.
  // Then, there will be inner cycles - up to matrixSize / 2
  int cycleCount = matrixSize / 2;

  for (int i = 0; i < cycleCount; i++) {
    for (int j = i; j < matrixSize - (i + 1); j++) {
      // swap elements by 90 degree in anticlockwise direction
      auto temp = matrix[i][j];
      // Right Column shifts up as Top row
      matrix[i][j] = matrix[j][matrixSize - 1 - i];
      // Bottom row moves up as Right column
      matrix[j][matrixSize - 1 - i] =  matrix[matrixSize - 1 - i][matrixSize - 1 - j];
      // Left Column shifts down as Bottow row
      matrix[matrixSize - 1 - i][matrixSize - 1 - j] = matrix[matrixSize - 1 - j][i];
      // Top row moves down as Left column
      matrix[matrixSize - 1 - j][i] = temp;
    }
  }

  printMatrix("After 90degree Clockwise Rotation", matrix);
}

void rotateClockwiseBy90Degree(vector<vector<int>>& matrix) {
  printMatrix("Original Matrix", matrix);

  int matrixSize = matrix.size();
  // The first cycle is formed by its first row, last column, last row, and first column.
  // Then, there will be inner cycles - up to matrixSize / 2
  int cycleCount = matrixSize / 2;

  for (int i = 0; i < cycleCount; i++) {
    for (int j = i; j < matrixSize - (i + 1); j++) {
      // swap elements by 90 degree in clockwise direction
      auto temp = matrix[i][j];
      // Left Column shifts up as Top row
      matrix[i][j] = matrix[matrixSize - 1 - j][i];
      // Bottom row moves up as Left column
      matrix[matrixSize - 1 - j][i] =  matrix[matrixSize - 1 - i][matrixSize - 1 - j];
      // Right Column shifts down as Bottow row
      matrix[matrixSize - 1 - i][matrixSize - 1 - j] = matrix[j][matrixSize - 1 - i];
      // Top row moves down as Right column
      matrix[j][matrixSize - 1 - i] = temp;
    }
  }

  printMatrix("After 90degree Clockwise Rotation", matrix);
}

void testRotateClockwiseBy90Degree() {
  std::vector<std::vector<int>> matrix { {1,  2,  3,  4,  5 },
                                         {6,  7,  8,  9,  10},
                                         {11, 12, 13, 14, 15},
                                         {16, 17, 18, 19, 20},
                                         {21, 22, 23, 24, 25}};
  auto rightRotate = matrix;
  // rotate clockwise by 90 degree
  rotateClockwiseBy90Degree(rightRotate);
  // rotate anticlockwise by 90 degree
  rotateAntiClockwiseBy90Degree(rightRotate);
  // Now rightRotate == matrix
  if (rightRotate != matrix) {
    std::cout << "Failed!!" << std::endl;
  } else {
    std::cout << "Success" << std::endl;
  }
}

void rotateAntiClockwise(vector<vector<int>>& matrix) {
  size_t rowCount = matrix.size();
  size_t colCount = matrix[0].size();
  size_t rowIndex = 0;
  size_t colIndex = 0;
  size_t times = std::min(rowCount, colCount) / 2;

  for (size_t row = 0; row < times; row++) {
    // Lets start with first remaining row.
    rowIndex = row;
    colIndex = row;
    int saved1 = matrix[rowIndex][colIndex];
    for (; colIndex < colCount - row - 1; colIndex++) {
      matrix[rowIndex][colIndex] = matrix[rowIndex][colIndex + 1];
    }

    // Next is the first remaining column.
    colIndex = row;
    rowIndex = rowCount - row - 1;
    int saved2 = matrix[rowIndex][colIndex];
    for (; rowIndex > row + 1; rowIndex--) {
      matrix[rowIndex][colIndex] = matrix[rowIndex - 1][colIndex];
    }
    matrix[rowIndex][colIndex] = saved1;

    // Next is the last remaining row.
    colIndex = colCount - row - 1;
    rowIndex = rowCount - row - 1;
    saved1 = matrix[rowIndex][colIndex];
    for (; colIndex > row + 1; colIndex--) {
      matrix[rowIndex][colIndex] = matrix[rowIndex][colIndex - 1];
    }
    matrix[rowIndex][colIndex] = saved2;

    // Last is the last remaining column.
    colIndex = colCount - row - 1;
    rowIndex = row;
    for (; rowIndex < rowCount - row - 2; rowIndex++) {
      matrix[rowIndex][colIndex] = matrix[rowIndex + 1][colIndex];
    }
    matrix[rowIndex][colIndex] = saved1;
  }
}

void matrixRotation(vector<vector<int>>& matrix, int r) {

  cout << "=================================\n";
  printMatrix("Original Matrix", matrix);
  cout << "=================================\n";

  cout << "rotationCount: " << r << endl;
  r = r % (2 * matrix.size() + 2 * matrix[0].size());
  for (int rotationCount = 0; rotationCount < r; rotationCount++) {
    rotateAntiClockwise(matrix);
    printMatrix("After Anticlokwise Rotation by 1", matrix);
    cout << "=================================\n";
  }

  printMatrix("After Anticlokwise Rotation", matrix);
}

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int main() {
  testRotateClockwiseBy90Degree();
  string mnr_temp;
  getline(cin, mnr_temp);

  vector<string> mnr = split(rtrim(mnr_temp));

  int m = stoi(mnr[0]);

  int n = stoi(mnr[1]);

  int r = stoi(mnr[2]);

  vector<vector<int>> matrix(m);

  for (int i = 0; i < m; i++) {
    matrix[i].resize(n);

    string matrix_row_temp_temp;
    getline(cin, matrix_row_temp_temp);

    vector<string> matrix_row_temp = split(rtrim(matrix_row_temp_temp));

    for (int j = 0; j < n; j++) {
      int matrix_row_item = stoi(matrix_row_temp[j]);

      matrix[i][j] = matrix_row_item;
    }
  }

  matrixRotation(matrix, r);

  return 0;
}

string ltrim(const string &str) {
  string s(str);

  s.erase(
    s.begin(),
    find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
  );

  return s;
}

string rtrim(const string &str) {
  string s(str);

  s.erase(
    find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
    s.end()
  );

  return s;
}

vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));

    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}
