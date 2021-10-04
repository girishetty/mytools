#ifndef _STACK_H
#define _STACK_H

class Node;

class Stack {
public:
    Stack() : iSize(0), iTop(NULL) {}

    bool IsEmpty() const { 
        return (iSize == 0);
    }
    
    int Size() const {
        return iSize;
    }

    int Pop();
    int Peek() const;
    void Push(int data);
    void Print() const;
    ~Stack();

private:
    void DeleteTop();

private:
    int   iSize;
    Node* iTop;
};

#endif //_STACK_H
