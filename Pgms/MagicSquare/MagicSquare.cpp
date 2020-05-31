#include <iostream>
#include "MagicSquare.h"

using std::cout;
using std::endl;
using std::vector;

MagicSquare::MagicSquare(size_t aSize) : iSize(aSize) {
  if (!(aSize & 0x01)) {
    iSize = aSize + 1;
  }
  for (int i = 0; i < iSize; i++) {
    for (int j = 0; j < iSize; j++) {
      iSquare[i][j] = 0;
    }
  }
}

void MagicSquare::Display() const {
  for (int i = 0; i < iSize; i++) {
    for (int j = 0; j < iSize; j++) {
      cout << "    " << iSquare[i][j];
    }
    cout << endl << endl;
  }
}

void MagicSquare::Create() {
  int row = 0;
  int col = (iSize) / 2;
  const int maxFill = iSize * iSize;
  int filled = 1;
  
  do {
    iSquare[row][col] = filled++;
    row = (row + iSize - 1) % iSize;
    col = (col + iSize - 1) % iSize;

    //If its already filled with some non-zero number
    if (iSquare[row][col] != 0) {
      row = (row + iSize + 2) % iSize;
      col = (col + iSize + 1) % iSize;
    }
  } while(filled <= maxFill);
}

// Return sum of difference between each element of two vector 
static size_t diff(const vector<int>& a, const vector<vector<int>>& b) {
  int res = 0;
  size_t index = 0;

  for (size_t row = 0; row < 3 ; row++) {
    for (size_t col = 0; col < 3 ; col++) {
      res += std::abs(a[index++] - b[row][col]);
    }
  }

  return res;
}

size_t MagicSquare::fixMagicSquare(vector<vector<int>>& square) {
  /**
   * A magic square looks something like this:
   * 6 1 8
   * 7 5 3
   * 2 9 4
   *
   * With that, there will be 8 combinations based on reflection and rotation
   * of above combinations as listed below:
   *         [8, 1, 6, 3, 5, 7, 4, 9, 2],
   *         [6, 1, 8, 7, 5, 3, 2, 9, 4],
   *         [4, 9, 2, 3, 5, 7, 8, 1, 6],
   *         [2, 9, 4, 7, 5, 3, 6, 1, 8],
   *         [8, 3, 4, 1, 5, 9, 6, 7, 2],
   *         [4, 3, 8, 9, 5, 1, 2, 7, 6],
   *         [6, 7, 2, 1, 5, 9, 8, 3, 4],
   *         [2, 7, 6, 9, 5, 1, 4, 3, 8]
   */

  vector<vector<int>> magicList = { {8, 1, 6, 3, 5, 7, 4, 9, 2},
                                    {6, 1, 8, 7, 5, 3, 2, 9, 4},
                                    {4, 9, 2, 3, 5, 7, 8, 1, 6},
                                    {2, 9, 4, 7, 5, 3, 6, 1, 8},
                                    {8, 3, 4, 1, 5, 9, 6, 7, 2},
                                    {4, 3, 8, 9, 5, 1, 2, 7, 6},
                                    {6, 7, 2, 1, 5, 9, 8, 3, 4},
                                    {2, 7, 6, 9, 5, 1, 4, 3, 8} };
  size_t cost = (size_t)-1;

  for (auto magic : magicList) {
    cost = std::min(cost, diff(magic, square));
  }

  return cost;
}
