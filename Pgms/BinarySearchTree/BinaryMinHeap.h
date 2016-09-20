#ifndef _BINARY_MIN_HEAP_H
#define _BINARY_MIN_HEAP_H

#include "BinaryHeap.h"

class BinaryMinHeap : public BinaryHeap {
public:
	BinaryMinHeap(int aSize = DEFAULT_MAX_HEAP_SIZE);
	virtual ~BinaryMinHeap();
	int RemoveMin() {
		return RemoveRoot();
	}

protected:
	virtual bool BinaryPropertyBroken(int index) const;
	virtual int BinaryFunction(int leftChildIndex, int rightChildIndex) const;
	virtual int Root() const;
};

#endif //_BINARY_MIN_HEAP_H
