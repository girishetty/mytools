#ifndef _SLOT_H
#define _SLOT_H

#include<list>
using namespace std;

class Entry;
class Data;

/*
 * Slot abstracts how Cache entris are stored, accessed and located (based on which replacement policy is used
 * For the simplicity, I am using Linked List based approach 
 * (hashed map along with List would make it better in terms of performance optimization, but its complicated to
 * design and implement with the time constraints
 *
 * For each Replacement Policy (LRU, MRU, FIFO, LIFO or any other), that has to derive from this class to give the
 * actual implementation
 */

class Slot {
public:
	Slot(int aSlots) : iNumSlots(aSlots), iCount(0) {}
	void Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize);
	void* Lookup(const void* aKey, int aKeySize);

private:
	virtual void AddEntry(Entry& anEntry) = 0;
	virtual void RemoveEntry() = 0;

private:
	Slot(const Slot& aSlot) : iNumSlots(aSlot.iNumSlots) {}
	Slot& operator=(const Slot& aSlot) {}

protected:
	const int iNumSlots;
	int iCount;
	list<Entry> iEntry;
};


/*
 * Abstraction for Data - that encapsulates pointer to data and the size info
 */

class Data {
public:
	Data(const void* aData, int aSize) : iData(aData), iSize(aSize) {}
	inline const void* GetData() const { return iData; }
	inline int Size() const { return iSize; }

private:
	const void* iData;
	int iSize;
};

/*
 * Abstraction for the entry, that basically holds Key and Value
 * And also, Valid and Dirtry bit - for future extension
 */

class Entry {
public:
	Entry(Data& aKey, Data& aValue) : iKey(aKey), iValue(aValue), iValid(1), iDirty(0) {} 
	Entry(const Entry& anEntry) : iKey(anEntry.iKey), iValue(anEntry.iValue), iValid(1), iDirty(anEntry.iDirty) {} 
	inline Data Key() const { return iKey; }
	inline Data Value() const { return iValue; }

private:
	Data iKey;
	Data iValue;
	unsigned char iValid;
	unsigned char iDirty;
};

#endif
