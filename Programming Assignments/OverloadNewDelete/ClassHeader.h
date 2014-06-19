#include <stddef.h>
#include <stdlib.h>

class NewClass {
private:
	int iData;
	NewClass* iNext;

#ifdef USE_FREE_NODE
	// pointer to free list
	static NewClass* iFreeList;             
#endif //USE_FREE_NODE

public:
	NewClass(int aData = 0 ) : iData(aData) { }
	~NewClass() { }
	void Set(int aData) { iData = aData; }
	int Get() const { return iData; }

#ifdef USE_FREE_NODE
	// overloaded new() and delete
	void* operator new(size_t aSize);     
	void operator delete(void* aPtr);
#endif //USE_FREE_NODE

};
