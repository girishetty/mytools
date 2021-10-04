/*
 * https://www.geeksforgeeks.org/n-queen-problem-backtracking-3/
 *
 * The N Queen is the problem of placing N chess queens on an N×N chessboard so that no two queens attack each other. For example, following is a solution for 4 Queen problem.
 *
 */

#include <vector>
#include <iomanip>
#include <iostream>

typedef std::vector<std::vector<int>> TwoDArray;

class ChessTable {
private:
  /*
   * For a Queen position {row, col}, it will update the ChessTable with:
   * - Marking all the illigal/invalid cells by putting -1
   * - Updating the {row, col} as used/placed.
   */
  void updateChessTable(int row, int col) {
    // First elminate all the cells in this row and column
    for (int i = 0; i < mSize; i++) {
      mTable[row][i] = -1;
      mTable[i][col] = -1;
    }

    int i = 0;
    int j = 0;
    // {top left to bottom right} diagonal line
    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
      mTable[i][j] = -1;
    }
    for (i = row + 1, j = col + 1; i < mSize && j < mSize; i++, j++) {
      mTable[i][j] = -1;
    }

    // {top right to bottom left} diagonal line
    for (i = row - 1, j = col + 1; i >= 0 && j < mSize; i--, j++) {
      mTable[i][j] = -1;
    }
    for (i = row + 1, j = col - 1; i < mSize && j >= 0; i++, j--) {
      mTable[i][j] = -1;
    }

    // Now place back Queen in {row, col}
    mTable[row][col] = 1;
  }

  /*
   * Places a Queen in a column (begin from given start column) for the given row if possible.
   * And also updates the ChessTable that eliminates all invalid cells
   * If we can't place the queue, it returns -1
   */
  int placeQueen(int row, int col) {
    bool found = false;

    for (; col < mSize; col++) {
      if (mTable[row][col] == 0) {
        found = true;
        break;
      }
    }

    if (found) {
      // Update the queen position and Eliminate all invalid moves/cells
      updateChessTable(row, col);
      return col;
    }

    return -1;
  }

public:
  ChessTable(int n) : mSize(n),
                      mTable (n, std::vector<int>(n, 0)) {
  }

  ~ChessTable() {
  }

  std::vector<int> placeNQueens(int n) {
    int attempts = 0;
    int attemptsMask = (1 << n) - 1;
    int queenIndex = 0;
    std::vector<int> queenList;
    std::vector<TwoDArray> tableList;

    // start with placing the first queen outside the Table, that is {-1}
    queenList.push_back(-1);
    // Clean Chess table state
    tableList.push_back(mTable);

    while (!queenList.empty()) {
      queenIndex = queenList.size() - 1;
      int placementIndex = queenList.back();
      queenList.pop_back();
      mTable = tableList.back();

#ifdef DEBUG
      if (queenIndex >= 0 && queenIndex < mSize && placementIndex >= 0 && placementIndex < mSize) {
        displayChessTable();
      }
#endif

      // Now place Queen (queenIndex) at next available column
      placementIndex = placeQueen(queenIndex, placementIndex + 1);
      if (placementIndex != -1) {
        // Working case.
        // Push the col position and the Chess Table State after placing this Queen for this row
        queenList.push_back(placementIndex);
        tableList.push_back(mTable);
        if (queenList.size() == n) {
          // We have sucessfully placed all the Queens
          break;
        }
        // Place the next queen outside the Table, that is {-1}
        queenList.push_back(-1);
      } else {
        // Go back to the last good state by removing the last Chess table state
        tableList.pop_back();

        // Exit condition
        attempts = attempts | (1 << queenIndex);
        if (attempts == attemptsMask) {
          break;
        }
      }
    }

    if (queenList.size() != n) {
      // Ensure its empty
      queenList.clear();
    }
    return queenList;
  }

  void displayChessTable() const {
    for (int i = 0; i < mSize; i++) {
      for (int j = 0; j < mSize; j++) {
        std::cout << std::setw(4) << mTable[i][j] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "==============================" << std::endl;
  }

private:
  int       mSize = 0;
  TwoDArray mTable;
};

void testNQueen(int n) {
  ChessTable table(n);
  auto queens = table.placeNQueens(n);

  if (queens.size() != n) {
    std::cout << "We can't place " << n << " Queens in " << n << " x " << n << " Chess Board" << std::endl;
    return;
  }

  std::cout << n << " Queens Can be placed in " << n << " x " << n << " Chess Board as below:" << std::endl;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (col == queens[row]) {
        std::cout << " 1 ";
      } else {
        std::cout << " 0 ";
      }
    }
    std::cout << std::endl;
  }
}

int main() {

  //testNQueen(5);
  //testNQueen(6);
  testNQueen(8);

  return 0;
}
