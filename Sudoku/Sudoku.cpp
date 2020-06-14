#include <iostream>
using namespace std;
/*
 * http://www.geeksforgeeks.org/archives/13779
 */

#define SUDOKU_SIZE 9

typedef unsigned int UINT;

class Sudoku {
public:
    Sudoku(UINT aSudokuGrid[SUDOKU_SIZE][SUDOKU_SIZE]) {
        memcpy(iSudokuGrid, aSudokuGrid, sizeof(UINT)*SUDOKU_SIZE*SUDOKU_SIZE);
    }

    bool SolvePuzzle();
    void PrintSudoku() const;

private:
    bool SolvePuzzle(UINT row, UINT col);
    bool IsValidEntry(UINT num, UINT row, UINT col) const;
    bool FindNextUnassignedSlot(UINT& row, UINT& col) const;

private:
    UINT iSudokuGrid[SUDOKU_SIZE][SUDOKU_SIZE];
};

#if 1
bool Sudoku::SolvePuzzle() {
    UINT row = 0;
    UINT col = 0;
    bool result = FindNextUnassignedSlot(row, col);
    if (result) {
        result = SolvePuzzle(row, col);
    }

    return result;
}
#else
bool Sudoku::SolvePuzzle() {
    UINT row, col;
    // If there is no unassigned cell, we are done
    if (!FindNextUnassignedSlot(row, col)) {
        return true;
    }

    for (int num = 1; num <= 9; num++) {
        // if looks promising
        if (IsValidEntry(num, row, col)) {
            // make tentative assignment
            iSudokuGrid[row][col] = num;

            // return, if success, yay!
            if (SolvePuzzle()) {
                return true;
            }

            // failure, unmake & try again
            iSudokuGrid[row][col] = 0;
        }
    }
    return false; // this triggers backtracking}
}
#endif

bool Sudoku::SolvePuzzle(UINT row, UINT col) {
    bool res = true;

    //See whether current cell is assigned or not
    if (iSudokuGrid[row][col] != 0) {
        //If not, find next unassigned cell
        res = FindNextUnassignedSlot(row, col);
    }

    if (res) {
        //We still have more to solve/fill
        for (int num = 1; num <= 9; num++) {
            if (IsValidEntry(num, row, col)) {
                //num is a potential entry in this cell, try with that once
                iSudokuGrid[row][col] = num;
                //try next slot
                int lastRow = row;
                int lastCol = col;
                col++;
                if (col == SUDOKU_SIZE) {
                    row++;
                    col = 0;
                }

                if (SolvePuzzle(row, col) == false) {
                    // On failure, backtrack by trying new value
                    row = lastRow;
                    col = lastCol;
                    iSudokuGrid[row][col] = 0;
                }
            }
        }

    }
    return true;
}

bool Sudoku::FindNextUnassignedSlot(UINT& row, UINT& col) const {
    for (;row < SUDOKU_SIZE; row++) {
        for (; col < SUDOKU_SIZE; col++) {
           if (iSudokuGrid[row][col] == 0) {
               return true;
           }
        }
        col = 0;
    }

    return false;
}

bool Sudoku::IsValidEntry(UINT num, UINT row, UINT col) const {
    UINT i=0;
    UINT j=0;
    //Check for all rows
    for (;i<SUDOKU_SIZE; i++) {
        if (iSudokuGrid[i][col] == num) {
            return false;
        }
    }

    //Check for all columns
    for (; j<SUDOKU_SIZE; j++) {
        if (iSudokuGrid[row][j] == num) {
            return false;
        }
    }

    //Check for 3*3 inner matrix
    UINT gridBeginRow = row - row % 3;
    UINT gridBeginCol = col - col % 3;
    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) {
            if (iSudokuGrid[gridBeginRow + i][gridBeginCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}

void Sudoku::PrintSudoku() const {
    cout<<"Sudoku Table"<<endl;
    for (UINT row = 0; row < SUDOKU_SIZE; row++) {
        for (UINT col = 0; col < SUDOKU_SIZE; col++) {
           cout<<iSudokuGrid[row][col]<<"  ";
        }
        cout<<endl;
    }
    cout<<endl<<endl;
}

int main() {
    UINT puzzle[SUDOKU_SIZE][SUDOKU_SIZE]={
        {3, 0, 6, 5, 0, 8, 4, 0, 0},
        {5, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 8, 7, 0, 0, 0, 0, 3, 1},
        {0, 0, 3, 0, 1, 0, 0, 8, 0},
        {9, 0, 0, 8, 6, 3, 0, 0, 5},
        {0, 5, 0, 0, 9, 0, 6, 0, 0},
        {1, 3, 0, 0, 0, 0, 2, 5, 0},
        {0, 0, 0, 0, 0, 0, 0, 7, 4},
        {0, 0, 5, 2, 0, 6, 3, 0, 0}
    };

    Sudoku sud(puzzle);
    sud.PrintSudoku();
    sud.SolvePuzzle();
    sud.PrintSudoku();
    sud.SolvePuzzle();
    sud.PrintSudoku();
    return 0;
}
