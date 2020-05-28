# define  USE_FREE_NODE

#include "ClassHeader.h"

#ifdef USE_FREE_NODE
NewClass* NewClass::iFreeList = NULL;

inline void* NewClass::operator new(size_t aSize) {
	// get free node from freelist if any
	if (iFreeList) {
		NewClass* ptr = iFreeList;
		iFreeList = iFreeList->iNext;
		return ptr;
	}

	// call malloc() otherwise
	return malloc(aSize);
}

inline void NewClass::operator delete(void* aPtr) {
	NewClass* ptr = (NewClass*) aPtr;
	// link freed node onto freelist
	ptr->iNext = iFreeList;
	iFreeList = ptr;
}

#endif //USE_FREE_NODE

int main() {
	const int FullList = 10;
	const int HalfList = 5;
	NewClass* pNewClass [FullList];
	int j;

	// Create as many as fullList elements in heap
	for (j = 0; j < FullList; j++)
		pNewClass[j] = new NewClass();
	
	// Delete all first halfList Count of Objects from Heap
	for (j = 0; j < HalfList; j++)
		delete pNewClass[j];

	// Create as many as halfList elements in heap
	for (j = 0; j < HalfList; j++)
		pNewClass[j] = new NewClass();
	
	// Delete all Objects in Heap
	for (j = 0; j < FullList; j++)
		delete pNewClass[j];

	return 0;
}
