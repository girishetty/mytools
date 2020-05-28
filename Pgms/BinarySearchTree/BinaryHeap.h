#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H

#define DEFAULT_MAX_HEAP_SIZE 100
#ifndef INT_MAX
#define INT_MAX 0x7FFFFFFF
#define INT_MIN 0xFFFFFFFF - 1
#endif //INT_MAX

void Display(int array[], int length);

class BinaryHeap {
public:
    BinaryHeap(int aSize);
    virtual ~BinaryHeap();
    bool Add(int aVal);
    int RemoveRoot();

    bool IsEmpty() const {
        return (iHeapArraySize == 0);
    }
    
    int Size() const {
        return iHeapArraySize;
    }

    void Display() const {
        ::Display(iHeapArray, iHeapArraySize);
    }

protected:
    virtual bool BinaryPropertyBroken(int index) const = 0 ;
    virtual int BinaryFunction(int leftChildIndex, int rightChildIndex) const = 0;
    virtual int Root() const = 0;

    void ShiftUp(int index);
    void ShiftDown(int index);

    int Root(int aDefault) const {
        int root = aDefault;
        if(iHeapArraySize > 0) {
            root = iHeapArray[0];
        }
        return root;
    }

    int LeftChildIndex(int index) const {
        return (index << 1) + 1;
    }
    int RightChildIndex(int index) const {
        return (index << 1) + 2;
    }
    int ParentIndex(int index) const {
        return (index - 1) >>  1;
    }
protected:
    int* iHeapArray;
    int  iHeapMaxSize;
    int  iHeapArraySize;
};

#endif //_BINARY_HEAP_H
