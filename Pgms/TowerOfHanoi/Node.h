#ifndef _NODE_H
#define _NODE_H

class Node {
public:
    Node(int data, Node* pNext = NULL) : iData(data), iNext(pNext) {}

    int Data() const {
        return iData;
    }

    void Next(Node* pNext) {
        iNext = pNext;
    }
    
    Node* Next() const {
        return iNext;
    }

private:
    int   iData;
    Node* iNext;
};

//global helper function to print list of Nodes
void Print(Node* node);

#endif //_NODE_H
