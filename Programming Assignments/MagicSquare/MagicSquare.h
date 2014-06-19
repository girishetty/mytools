#ifndef MAGICSQUARE_H
#define MAGICSQUARE_H

#include <iostream>

const int KMaxSize = 20;

class MagicSquare {
public:
	MagicSquare(int aSize = 1) : iSize(aSize) {
		if(!(aSize & 0x01)) {
			iSize = aSize + 1;
		}
		for(int i=0; i<iSize; i++)
			for(int j=0; j<iSize; j++)
				iSquare[i][j] = 0;
	}

	void Display() {
		for(int i=0; i<iSize; i++) {
			for(int j=0; j<iSize; j++) {
				cout<<"    "<<iSquare[i][j];
			}
			cout<<endl<<endl;
		}
	}

	void Create() {
		int row = 0;
		int col = (iSize)/2;
		const int maxFill = iSize * iSize;
		int filled = 1;
		
		do{
			iSquare[row][col] = filled++;
			row = (row+iSize-1)%iSize;
			col = (col+iSize-1)%iSize;

			//If its already filled with some non-zero number
			if(iSquare[row][col] != 0) {
				row = (row+iSize+2)%iSize;
				col = (col+iSize+1)%iSize;
			}
		}while(filled <= maxFill);
	}

private:
	int		iSize;
	int		iSquare[KMaxSize][KMaxSize];
};

#endif //MAGICSQUARE_H
