#include "HeapSort.h"

inline int LeftChildIndex(int index) {
    return (index << 1) + 1;
}
inline int RightChildIndex(int index) {
    return (index << 1) + 2;
}
inline int ParentIndex(int index) {
    return (index - 1) >>  1;
}

void MaxHeapify(int array[], int length, int index) {
    int left = LeftChildIndex(index);
    int right = RightChildIndex(index);
    int largest = index;

    //Find the largest of three
    if (left < length && array[left] > array[largest]) {
        largest = left;
    }
    if (right < length && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != index) {
        int temp = array[index];
        array[index] = array[largest];
        array[largest] = temp;
        MaxHeapify(array, length, largest);
    }
}

void BuildMaxHeap(int array[], int length) {
    //For all non-leaf nodes, call MaxHeapify from bottom up approach
    int lastNonLeafNode = ParentIndex(length-1);
    for (int index = lastNonLeafNode; index >= 0; index--) {
        MaxHeapify(array, length, index);
    }
}

void HeapSort(int array[], int length) {
    //Build heap which pushes the max element on to the root (begining of the list)
    BuildMaxHeap(array, length);
    int maxElement = 0;

    for (int index = length - 1; index > 0; index--) {
        //Now, move the Max element (in the root) to the last index by swaping 0th and last element
        maxElement = array[0];
        array[0] = array[index];
        array[index] = maxElement;
        //Rerun the MaxHeapify with one lesser length to get the next max element
        MaxHeapify(array, index, 0);
    }
}
