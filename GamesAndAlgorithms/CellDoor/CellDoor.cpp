#include <iostream>
#include "CellDoor.h"

CellDoor::CellDoor() {
  for (int i = 0; i < NUM_DOORS; i++)
    Doors[i] = true;
}

CellDoor::~CellDoor() {
}

void CellDoor::solvePuzzle() {
  int i, j;
  int copID;

  for (i = 1; i < NUM_DOORS; i++) {
    copID = i+1;
    for (j = copID; j < NUM_DOORS; j += copID) {
      if ((j) % (copID) == 0) {
        Doors[j-1] = ! Doors[j-1];
      }
    }
  }
}

void CellDoor::displayResult() const {
  for (int i = 0; i < NUM_DOORS; i++) {
    if(Doors[i] == true) {
      printf("\tDoor %.3d is OPENED\n", i+1);
    } else {
      printf("Door %.3d is CLOSED\n", i+1);
    }
  }
}


void main () {
  CellDoor cell;

  cell.solvePuzzle();
  cell.displayResult();
}
