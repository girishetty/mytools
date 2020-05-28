#include <iostream>
using namespace std;
#include "BaseWithPV.h"

int main() {
    cout<<endl<<"Construct Stack Based Derived object..."<<endl;
    Derived d;
    d.Clean();
    cout<<endl<<"Construct Heap Based Derived object..."<<endl;
    Base* pB = new Derived();
    pB->Clean();
    cout<<endl<<"Delete Heap Based Derived object..."<<endl;
    delete pB;
    cout<<endl<<"Delete Stack Based Derived object..."<<endl;
    return 0;
}
