#include <iostream>
#include "Node.h"
using namespace std;

void Print(Node* node) {
    Node* next = node->Next(); 
    if (next) {
        Print(next);
    }
    cout<<node->Data()<<endl;
}
