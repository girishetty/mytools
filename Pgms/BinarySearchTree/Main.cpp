#include <iostream>
using namespace std;
#include "BST.h"
#include "BinaryMinHeap.h"
#include "BinaryMaxHeap.h"
#include "HeapSort.h"

void BinarySearchTreeTest() {
    BinarySearchTree* pBST = new BinarySearchTree;
    pBST->Add(8);
    pBST->Display();
    pBST->Add(4);
    pBST->Display();
    pBST->Add(7);
    pBST->Add(9);
    pBST->Display();
    pBST->Add(11);
    pBST->Add(5);
    pBST->Add(13);
    pBST->Display();
    pBST->Add(3);
    pBST->Add(19);
    pBST->Add(1);
    pBST->Display();

    pBST->Remove(13);
    pBST->Display();

    if(pBST->Search(13)) {
        cout<<"Found 13.."<<endl;
    }
    if(pBST->Search(3)) {
        cout<<"Found 3.."<<endl;
    }

    cout<<"Root is : "<<pBST->Root()<<endl;
    pBST->Remove(8);
    pBST->Display();
    cout<<"Root is : "<<pBST->Root()<<endl;
    
    delete pBST;
}

void BinaryMinMaxHeapTest(bool minHeap) {
    int size = 10;
    int val = 0;
    BinaryHeap* pBH = NULL;
    if(minHeap) {
        pBH = new BinaryMinHeap(size);
    }
    else {
        pBH = new BinaryMaxHeap(size);
    }

    for(int i=0; i<size; i++) {
        val = rand() % size;
        pBH->Add(val);
        if((i+1)%5 == 0) {
            pBH->Display();
        }
    }

    for(int i=0; i<size; i++) {
        val = pBH->RemoveRoot();
        cout<<"Removed Element : "<<val<<endl;
        pBH->Display();
    }
    delete pBH;
}

void BinaryMaxHeapTest() {
    int size = 20;
    int val = 0;
    BinaryMaxHeap* pBH = new BinaryMaxHeap(size);

    for(int i=0; i<size; i++) {
        val = rand() % size;
        pBH->Add(val);
    }

    cout<<"Before Sorting..."<<endl;
    pBH->Display();
    pBH->HeapSort();
    cout<<"After Sorting..."<<endl;
    pBH->Display();

    delete pBH;
}

void HeapSortTest() {
    int size = 20;
    int val = 0;
    int* pArray = new int[size];

    cout<<"Before Sorting: Array : ";
    for(int i=0; i<size; i++) {
        val = rand() % size;
        pArray[i] = val;
    }
    Display(pArray, size);
    HeapSort(pArray, size);
    cout<<"After Sorting: Array : ";
    Display(pArray, size);
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (strncmp(argv[1], "-s", 2) == 0 || strncmp(argv[1], "-a", 2) == 0) {
            BinarySearchTreeTest();
        }
        if (strncmp(argv[1], "-x", 2) == 0 || strncmp(argv[1], "-a", 2) == 0) {
            BinaryMinMaxHeapTest(true);
        }
        if (strncmp(argv[1], "-m", 2) == 0 || strncmp(argv[1], "-a", 2) == 0) {
            BinaryMinMaxHeapTest(false);
        }
        if (strncmp(argv[1], "-b", 2) == 0 || strncmp(argv[1], "-a", 2) == 0) {
            BinaryMaxHeapTest();
        }
        if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "-a", 2) == 0) {
            HeapSortTest();
        }
    }
    return 0;
}
