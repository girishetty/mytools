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
#include <iostream>
#include <iomanip>

using namespace std;

void printMatrix(const vector<vector<int>>& matrix) {
  for (const auto& row : matrix) {
    for (auto item : row) {
      cout << setw(4) << item << "    ";
    }
    cout << endl;
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
    cout << "[" << rowIndex << ":" << colIndex << ":" << saved1 << "]" << endl;
    for (; colIndex < colCount - row - 1; colIndex++) {
      matrix[rowIndex][colIndex] = matrix[rowIndex][colIndex + 1];
    }

    // Next is the first remaining column.
    colIndex = row;
    rowIndex = rowCount - row - 1;
    int saved2 = matrix[rowIndex][colIndex];
    cout << "[" << rowIndex << ":" << colIndex << ":" << saved2 << "]" << endl;
    for (; rowIndex > row + 1; rowIndex--) {
      matrix[rowIndex][colIndex] = matrix[rowIndex - 1][colIndex];
    }
    matrix[rowIndex][colIndex] = saved1;

    // Next is the last remaining row.
    colIndex = colCount - row - 1;
    rowIndex = rowCount - row - 1;
    saved1 = matrix[rowIndex][colIndex];
    cout << "[" << rowIndex << ":" << colIndex << ":" << saved1 << "]" << endl;
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
  printMatrix(matrix);
  cout << "=================================\n";

  cout << "rotationCount: " << r << endl;
  for (int rotationCount = 0; rotationCount < r; rotationCount++) {
    rotateAntiClockwise(matrix);
    printMatrix(matrix);
    cout << "=================================\n";
  }

  printMatrix(matrix);
}

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

int main() {
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
