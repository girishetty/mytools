#ifndef _QUEUE_H
#define _QUEUE_H

class Node;

class Queue {
public:
    Queue() : iSize(0), iFirst(NULL), iLast(NULL) {}

    bool IsEmpty() const { 
        return (iSize == 0);
    }
    
    int Size() const {
        return iSize;
    }

    void Enqueue(int data);
    int Dequeue();
    int Peek() const;
    void Print() const;
    ~Queue();

private:
    void DeleteFirst();
private:
    int   iSize;
    Node* iFirst;
    Node* iLast;
};

#endif //_QUEUE_H

