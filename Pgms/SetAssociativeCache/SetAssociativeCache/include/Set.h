#ifndef _SET_H
#define _SET_H

#include "..\include\Cache.h"

class Slot;

class Set {
public:
	Set();
	~Set();
	void Init(int aSlots, ReplacementPolicy aPolicy);
	void Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize);
	void* Lookup(const void* aKey, int aKeySize);

private:
	Set(const Set& aSet) {}
	Set& operator=(const Set& aSet) {}

private:
	int iNumSlots;
	Slot* iSlots;
};

#endif //_SET_H
