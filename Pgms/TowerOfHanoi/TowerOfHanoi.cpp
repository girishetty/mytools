#include <iostream>
#include "Tower.h"
using namespace std;

void Tower::Add(int disk) {
    if (!iDisks.IsEmpty() && iDisks.Peek() <= disk) {
        cout<<"Error while pushing disk : "<<disk<<endl;
    }
    else {
        iDisks.Push(disk);
    }
}

void Tower::MoveTopTo(Tower* t) {
    int top = iDisks.Pop();
    t->Add(top);
    cout<<"Move disk "<<top<<" from "<<Index()<<" to "<<t->Index()<<endl;
}

void Tower::MoveDisks(int n, Tower* aDest, Tower* aTemp) {
    if (n > 0) {
        MoveDisks(n - 1, aTemp, aDest);
        MoveTopTo(aDest);
        aTemp->MoveDisks(n - 1, aDest, this);
    }
}

void Tower::Print() const{
    cout<<"Contents of Tower "<<iIndex<<endl;
    iDisks.Print();
}
