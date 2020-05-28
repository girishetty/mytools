#include <iostream>
using namespace std;

# define MATRIX_SIZE 7

//represents one element with row and column index
typedef struct {
	int row;
	int col;
}Element;

//represents Seed Point Region with possibly MATRIX_SIZE*MATRIX_SIZE elements
Element seedPointRegion[MATRIX_SIZE*MATRIX_SIZE] = {0};
int count = 0;
bool visited[MATRIX_SIZE][MATRIX_SIZE] = {false};

void FindSeedPointRegion(int** pArray, Element element, int threshold);
int GetThreshold(int* pArray);

/*
Input: Pointer to an Array of size MATRIX_SIZE*MATRIX_SIZE
       Start points as row and column index indicated by Element
       threshold is a value representing threshould variation in the neighboring elements
*/
void FindSeedPointRegion(int* pArray, Element element, int threshold) {
	int neighbors[8];

	int startRow = element.row;
	int startCol = element.col;

	//Ignore this element if it belongs to the border elements, as it wont be having 8 enighbors
	if(startRow == 0 || startRow == MATRIX_SIZE-1 || startCol == 0 || startCol == MATRIX_SIZE-1) {
		return;
	}
	//Check whether we have visited this element already
	if(visited[startRow][startCol]) {
		return;
	}

	visited[startRow][startCol] = true;

	neighbors[0] = pArray [startRow-1 + startCol-1];
	neighbors[1] = pArray[startRow-1 + startCol];
	neighbors[2] = pArray[startRow-1 + startCol+1];

	neighbors[3] = pArray[startRow + startCol-1];
	neighbors[4] = pArray[startRow + startCol+1];

	neighbors[5] = pArray[startRow+1 + startCol-1];
	neighbors[6] = pArray[startRow+1 + startCol];
	neighbors[7] = pArray[startRow+1 + startCol+1];

	int thisThreshold = GetThreshold(neighbors);

	if(thisThreshold <= threshold) {
		//This element is in a seed point and all the elements in the neighbor list belogs to the regoin
		//So, add the elements and its neighbors to seedPointRegion and call FindSeedPointRegion on that
		seedPointRegion[count++] = element;

		//Neighbour#1
		seedPointRegion[count].row = startRow-1;
		seedPointRegion[count].col = startCol-1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#2
		count++;
		seedPointRegion[count].row = startRow-1;
		seedPointRegion[count].col = startCol;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#3
		count++;
		seedPointRegion[count].row = startRow-1;
		seedPointRegion[count].col = startCol+1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#4
		count++;
		seedPointRegion[count].row = startRow;
		seedPointRegion[count].col = startCol-1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#5
		count++;
		seedPointRegion[count].row = startRow;
		seedPointRegion[count].col = startCol+1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#6
		count++;
		seedPointRegion[count].row = startRow+1;
		seedPointRegion[count].col = startCol-1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#7
		count++;
		seedPointRegion[count].row = startRow+1;
		seedPointRegion[count].col = startCol;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);

		//Neighbour#8
		count++;
		seedPointRegion[count].row = startRow+1;
		seedPointRegion[count].col = startCol+1;
		FindSeedPointRegion(pArray, seedPointRegion[count],threshold);
	}
}

/*
Input:  Pointer to an Array of size 8
Output: Gets the difference between Max and Min element in the list
*/
int GetThreshold(int* pArray) {
	int max = pArray[0];
	int min = pArray[0];

	for(int index = 1; index < 8; index++) {
		if(pArray[index] < min) {
			min = pArray[index];
		}
		else if(pArray[index] > max) {
			max = pArray[index];
		}
	}

	return (max - min);
}

int main() {
	//initialize tha array here, I am too lazy, I cant do it :)
	int array[7][7] = {10};
	Element startElement = { 3, 4 };

	FindSeedPointRegion((int*)&array[0][0], startElement, 4);

	cout<<"I am done"<<endl;
	return 0;
}
