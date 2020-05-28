# include "stdio.h"

# define NUM_DOORS 100

class CellDoor
{
private:
	bool Doors [NUM_DOORS];

public:
	CellDoor ();
	~CellDoor ();

	solvePuzzle ();
	displayResult ();
};
