#ifndef _TOWER_H
#define _TOWER_H

#include "Stack.h"

class Tower {
private:
	Stack iDisks;
	int   iIndex;

public:
	Tower(const int aIndex) : iIndex(aIndex) {}
	
	int Index() const {
		return iIndex;
	}
	
	void Add(int disk);
	void MoveTopTo(Tower* t);
	void MoveDisks(int n, Tower* aDest, Tower* aTemp);
	void Print() const;
};

#endif //_TOWER_H
