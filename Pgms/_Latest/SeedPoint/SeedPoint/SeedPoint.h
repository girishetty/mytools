
# define MATRIX_SIZE 7

//represents one element with row and column index
typedef struct {
	int row;
	int col;
}Element;

//represents Seed Point Region with possibly MATRIX_SIZE*MATRIX_SIZE elements
Element seedPointRegion[MATRIX_SIZE*MATRIX_SIZE] = {0};

void FindSeedPointRegion(int** pArray, Element element, int threshold)
int GetThreshold(int* pArray);
