#include <iostream>
using namespace std;
#include "BinaryMaxHeap.h"

BinaryMaxHeap::BinaryMaxHeap(int aSize) : BinaryHeap(aSize){
}

BinaryMaxHeap::~BinaryMaxHeap() {
}

bool BinaryMaxHeap::BinaryPropertyBroken(int index) const {
  bool broken = false;
  if(index != 0 ) {
    int parentIndex = ParentIndex(index);
    if(iHeapArray[parentIndex] < iHeapArray[index]) {
      //Binary max tree property is broken
      broken = true;
    }
  }
  return broken;
}

int BinaryMaxHeap::BinaryFunction(int leftChildIndex, int rightChildIndex) const {
  //Find the max of left and right child
  int maxValIndex = leftChildIndex;
  if(iHeapArray[leftChildIndex] < iHeapArray[rightChildIndex]) {
    maxValIndex = rightChildIndex;
  }
  return maxValIndex;
}

int BinaryMaxHeap::Root() const {
  return BinaryHeap::Root(INT_MIN);
}

void BinaryMaxHeap::HeapSort() {
  int maxElement = 0;
  int actualLength = iHeapArraySize;

  for (int index = actualLength - 1; index > 0; index--) {
    //Now, move the Max element (in the root) to the last index by swaping 0th and last element
    maxElement = iHeapArray[0];
    iHeapArray[0] = iHeapArray[index];
    iHeapArray[index] = maxElement;
    //Rerun the ShiftDown with one lesser length to get the next max element
    iHeapArraySize--;
    ShiftDown(0);
  }
  //Array is sorted now, restore back the actual length
  iHeapArraySize = actualLength;
}
