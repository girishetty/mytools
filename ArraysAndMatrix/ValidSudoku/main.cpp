/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/92/array/769/
 *
 * Determine if a 9 x 9 Sudoku board is valid. Only the filled cells need to be validated according to the following rules:
 *
 * Each row must contain the digits 1-9 without repetition.
 * Each column must contain the digits 1-9 without repetition.
 * Each of the nine 3 x 3 sub-boxes of the grid must contain the digits 1-9 without repetition.
 * Note:
 *
 * A Sudoku board (partially filled) could be valid but is not necessarily solvable.
 * Only the filled cells need to be validated according to the mentioned rules.
 *
 * Example#1:
 * Input: board = 
 *    [['5','3','.','.','7','.','.','.','.']
 *    ,['6','.','.','1','9','5','.','.','.']
 *    ,['.','9','8','.','.','.','.','6','.']
 *    ,['8','.','.','.','6','.','.','.','3']
 *    ,['4','.','.','8','.','3','.','.','1']
 *    ,['7','.','.','.','2','.','.','.','6']
 *    ,['.','6','.','.','.','.','2','8','.']
 *    ,['.','.','.','4','1','9','.','.','5']
 *    ,['.','.','.','.','8','.','.','7','9']]
 * Output: true
 *
 * Example#2:
 * Input: board = 
 *    [['8','3','.','.','7','.','.','.','.']
 *    ,['6','.','.','1','9','5','.','.','.']
 *    ,['.','9','8','.','.','.','.','6','.']
 *    ,['8','.','.','.','6','.','.','.','3']
 *    ,['4','.','.','8','.','3','.','.','1']
 *    ,['7','.','.','.','2','.','.','.','6']
 *    ,['.','6','.','.','.','.','2','8','.']
 *    ,['.','.','.','4','1','9','.','.','5']
 *    ,['.','.','.','.','8','.','.','7','9']]
 * Output: false
 * Explanation: Same as Example 1, except with the 5 in the top left corner being modified to 8. Since there are two 8's in the top left 3x3 sub-box, it is invalid.
 *
 */

#include <cctype>
#include <vector>
#include <iostream>

bool isValidSudoku(const std::vector<std::vector<char>>& board) {
  // Check each row first
  for (const auto& row : board) {
    std::vector<bool> digits(10, false);
    for (const auto& entry : row) {
      if (std::isdigit(entry)) {
        int index = entry - 30;
        if (digits[index]) {
          return false;
        }
        digits[index] = true;
      }
    }
  }

  // Check each colums next
  for (int col = 0; col < 9; col++) {
    std::vector<bool> digits(10, false);
    for (const auto& row : board) {
      auto entry = row[col];
      if (std::isdigit(entry)) {
        int index = entry - 30;
        if (digits[index]) {
          return false;
        }
        digits[index] = true;
      }
    }
  }

  // Check each grid now
  int gridRow = 0;
  int gridCol = 0;
  for (; gridRow < 9; ) {
    std::vector<bool> digits(10, false);
    for (int rowSize = 0; rowSize < 3; rowSize++) {
      for (int colSize = 0; colSize < 3; colSize++) {
        auto entry = board[gridRow + rowSize][gridCol + colSize];
        if (std::isdigit(entry)) {
          int index = entry - 30;
          if (digits[index]) {
            return false;
          }
          digits[index] = true;
        }
      }
    }
    gridCol += 3;
    if (gridCol == 9) {
      gridCol = 0;
      gridRow += 3;
    }
  }

  return true;
}

void testIsValidSudoku(const std::vector<std::vector<char>>& board) {
  auto valid = isValidSudoku(board);
  if (valid) {
    std::cout << "Given Sudoko Board is Valid" << std::endl;
  } else {
    std::cout << "Given Sudoko Board is Not Valid" << std::endl;
  }
}

int main() {
  testIsValidSudoku(
                    {{'5','3','.','.','7','.','.','.','.'}
                    ,{'6','.','.','1','9','5','.','.','.'}
                    ,{'.','9','8','.','.','.','.','6','.'}
                    ,{'8','.','.','.','6','.','.','.','3'}
                    ,{'4','.','.','8','.','3','.','.','1'}
                    ,{'7','.','.','.','2','.','.','.','6'}
                    ,{'.','6','.','.','.','.','2','8','.'}
                    ,{'.','.','.','4','1','9','.','.','5'}
                    ,{'.','.','.','.','8','.','.','7','9'}}
                   );
  testIsValidSudoku(
                    {{'8','3','.','.','7','.','.','.','.'}
                    ,{'6','.','.','1','9','5','.','.','.'}
                    ,{'.','9','8','.','.','.','.','6','.'}
                    ,{'8','.','.','.','6','.','.','.','3'}
                    ,{'4','.','.','8','.','3','.','.','1'}
                    ,{'7','.','.','.','2','.','.','.','6'}
                    ,{'.','6','.','.','.','.','2','8','.'}
                    ,{'.','.','.','4','1','9','.','.','5'}
                    ,{'.','.','.','.','8','.','.','7','9'}}
                   );
  testIsValidSudoku(
                    {{'5','3','.','.','7','.','.','.','.'}
                    ,{'6','.','.','1','9','5','.','.','.'}
                    ,{'.','9','3','.','.','.','.','6','.'}
                    ,{'8','.','.','.','6','.','.','.','3'}
                    ,{'4','.','.','8','.','3','.','.','1'}
                    ,{'7','.','.','.','2','.','.','.','6'}
                    ,{'.','6','.','.','.','.','2','8','.'}
                    ,{'.','.','.','4','1','9','.','.','5'}
                    ,{'.','.','.','.','8','.','.','7','9'}}
                   );
}
