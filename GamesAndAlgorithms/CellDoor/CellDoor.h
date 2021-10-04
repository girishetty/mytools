#pragma once

# define NUM_DOORS 100

class CellDoor {
private:
  bool Doors [NUM_DOORS];

public:
  CellDoor ();
  ~CellDoor ();

  void solvePuzzle ();
  void displayResult () const;
};
