#include <iostream>
#include "Node.h"
#include "Queue.h"
using namespace std;

Queue::~Queue() {
    //Cleanup the stack
    for (;iSize != 0;) {
        DeleteFirst();
    }
}

void Queue::DeleteFirst() {
    //Delete the first element in the queue
    Node* temp = iFirst;
    iFirst = iFirst->Next();
    delete temp;
    //reduce the size by 1
    iSize--;
}

int Queue::Dequeue() {
    int data = -1;
    if (iFirst) {
        data = iFirst->Data();
        DeleteFirst();
    }
    return data;
}

int Queue::Peek() const {
    int data = -1;
    if (iFirst) {
        data = iFirst->Data();
    }
    return data;
}

void Queue::Enqueue(int data) {
    Node* newNode = new Node(data);
    if (!iFirst) {
        iLast = iFirst = newNode; 
    } else {
        iLast->Next(newNode);
        iLast = newNode;
    }
    iSize++;
}

void Queue::Print() const {
    if (iSize != 0 ) {
        //call global print function
        ::Print(iFirst);
    }
    else {
        cout<<"Empty"<<endl;
    }
}
