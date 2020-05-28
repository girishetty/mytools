#ifndef _BINARY_MAX_HEAP_H
#define _BINARY_MAX_HEAP_H

#include "BinaryHeap.h"

class BinaryMaxHeap : public BinaryHeap {
public:
    BinaryMaxHeap(int aSize = DEFAULT_MAX_HEAP_SIZE);
    virtual ~BinaryMaxHeap();
    int RemoveMax() {
        return RemoveRoot();
    }

    void HeapSort();

protected:
    virtual bool BinaryPropertyBroken(int index) const;
    virtual int BinaryFunction(int leftChildIndex, int rightChildIndex) const;
    virtual int Root() const;
};

#endif //_BINARY_MAX_HEAP_H
