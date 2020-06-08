#include <iostream>
#include "Node.h"
#include "Stack.h"
using namespace std;

Stack::~Stack() {
    //Cleanup the stack
    for (;iSize != 0;) {
        DeleteTop();
    }
}

void Stack::DeleteTop() {
    //Delete the element in the Top
    Node* temp = iTop;
    iTop = iTop->Next();
    delete temp;
    //reduce the size by 1
    iSize--;
}

int Stack::Pop() {
    int data = -1;
    if (iTop) {
        data = iTop->Data();
        DeleteTop();
    }
    return data;
}

int Stack::Peek() const {
    int data = -1;
    if (iTop) {
        data = iTop->Data();
    }
    return data;
}

void Stack::Push(int data) {
    Node* node = new Node(data, iTop);
    iTop = node;
    iSize++;
}

void Stack::Print() const {
    if (iSize != 0 ) {
        //call global print function
        ::Print(iTop);
    }
    else {
        cout<<"Empty"<<endl;
    }
}
