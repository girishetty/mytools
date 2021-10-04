#include "BinaryHeap.h"

BinaryHeap::BinaryHeap(int aSize) : iHeapMaxSize(aSize), iHeapArraySize(0) {
  iHeapArray = new int [aSize];
}

BinaryHeap::~BinaryHeap() {
  delete [] iHeapArray;
}

bool BinaryHeap::Add(int aVal) {
  bool success = false;
  if(iHeapArraySize < iHeapMaxSize) {
    //Add the new item to the end of the array
    iHeapArray[iHeapArraySize] = aVal;
    //Shift up the element to make sure binary tree property is not broken (parent <> both the children)
    ShiftUp(iHeapArraySize);
    iHeapArraySize++;
    success = true;
  }
  return success;
}

void BinaryHeap::ShiftUp(int index) {
  if(index != 0 ) {
    if(BinaryPropertyBroken(index)) {
      int parentIndex = ParentIndex(index);
      //Binary tree property has broken, so shift the element up to its parent
      int temp = iHeapArray[parentIndex];
      iHeapArray[parentIndex] = iHeapArray[index];
      iHeapArray[index] = temp;
      //Do shiftup on the parent index now
      ShiftUp(parentIndex);
    }
  }
}

int BinaryHeap::RemoveRoot() {
  int root = Root();
  if(iHeapArraySize > 0) {
    //Root has the min/max value in the binary heap
    iHeapArraySize--;
    if(iHeapArraySize > 0) {
      iHeapArray[0] = iHeapArray[iHeapArraySize];
      ShiftDown(0);
    }
  }
  return root;
}

void BinaryHeap::ShiftDown(int index) {
  int leftChildindex = LeftChildIndex(index);
  int rightChildindex = RightChildIndex(index);
  //Make left child as the key (min or max) value by default
  int keyIndex = leftChildindex;

  if(rightChildindex >= iHeapArraySize) {
    //No right child, so left will be the one, if ever its present
    if(leftChildindex >= iHeapArraySize) {
      //No left Child, so we are done
      return;
    }
  }
  else {
    //Both left and right child are present, so find the key value (min/max) of left and right child
    keyIndex = BinaryFunction(leftChildindex, rightChildindex);
  }
  //By now we have decided which one is key, check whether the property is broken
  if(BinaryPropertyBroken(keyIndex)) {
    //Binary tree property has broken, so shift the element down to the keyIndex (left or right child)
    int temp = iHeapArray[index];
    iHeapArray[index] = iHeapArray[keyIndex];
    iHeapArray[keyIndex] = temp;
    //After swapping do the shift down on the bigger index
    ShiftDown(keyIndex);
  }
}
