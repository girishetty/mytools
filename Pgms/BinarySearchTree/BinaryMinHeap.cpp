#include <iostream>
using namespace std;
#include "BinaryMinHeap.h"

BinaryMinHeap::BinaryMinHeap(int aSize) : BinaryHeap(aSize){
}

BinaryMinHeap::~BinaryMinHeap() {
}

bool BinaryMinHeap::BinaryPropertyBroken(int index) const {
    bool broken = false;
    if(index != 0 ) {
        int parentIndex = ParentIndex(index);
        if(iHeapArray[parentIndex] > iHeapArray[index]) {
            //Binary min tree property is broken
            broken = true;
        }
    }
    return broken;
}

int BinaryMinHeap::BinaryFunction(int leftChildIndex, int rightChildIndex) const {
    //Find the min of left and right child
    int minValIndex = leftChildIndex;
    if(iHeapArray[leftChildIndex] > iHeapArray[rightChildIndex]) {
        minValIndex = rightChildIndex;
    }
    return minValIndex;
}

int BinaryMinHeap::Root() const {
    return BinaryHeap::Root(INT_MAX);
}
