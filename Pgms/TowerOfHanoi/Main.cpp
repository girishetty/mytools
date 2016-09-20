#include <iostream>
#include "Tower.h"
using namespace std;

int main() {
    Tower** pTowers = new Tower*[3];
    for (int i=0; i<3; i++) {
        pTowers[i] = new Tower(i);
    }

    //Let 0th tower have all the N disks
    int no_of_disks = 3;
    for (int disk = no_of_disks; disk > 0; disk--) {
        pTowers[0]->Add(disk);
    }

    for (int i=0; i<3; i++) {
        pTowers[i]->Print();
    }

    //Move all the disks from 0th tower to 2nd tower
    pTowers[0]->MoveDisks(no_of_disks, pTowers[2], pTowers[1]);

    for (int i=0; i<3; i++) {
        pTowers[i]->Print();
    }

    //cleanup Towers
    for (int i=0; i<3; i++) {
        delete pTowers[i];
    }
    delete []pTowers;

    return 0;
}
