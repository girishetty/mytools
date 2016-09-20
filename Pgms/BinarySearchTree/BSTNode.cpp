#include <iostream>
using namespace std;
#include "BSTNode.h"

void BSTNode::Delete() {
    if (iLeft) {
        iLeft->Delete();
    }
    if (iRight) {
        iRight->Delete();
    }
    delete this;
}

bool BSTNode::Add(int aVal) {
    bool success = false;

    if (aVal < iValue) {
        if (iLeft) {
            success = iLeft->Add(aVal);
        } else {
            //Add this as left child of current node
            iLeft = new BSTNode(aVal);
            success = true;
        }
    } else if (aVal > iValue) {
        if (iRight) {
            success = iRight->Add(aVal);
        } else {
            //Add this as right child of current node
            iRight = new BSTNode(aVal);
            success = true;
        }
    }

    return success;
}

BSTNode* BSTNode::Remove(int aVal, BSTNode* aParent) {
    BSTNode* pNode = NULL;

    if (aVal < iValue) {
        if (iLeft) {
            pNode = iLeft->Remove(aVal, this);
        }
    } else if (aVal > iValue) {
        if (iRight) {
            pNode = iRight->Remove(aVal, this);
        }
    } else {
        if (iLeft && iRight) {
            //has both the children, so find the node with the min node from right half and make it as the current node
            iValue = iRight->MinValue();
            //Delete the duplicate node
            pNode = iRight->Remove(iValue, this);
        } else if (aParent->iLeft == this) {
            aParent->iLeft = iLeft ? iLeft : iRight;
            pNode = this;
        } else if (aParent->iRight == this) {
            aParent->iRight = iLeft ? iLeft : iRight;
            pNode = this;
        }
    }

    return pNode;
}

bool BSTNode::Search(int aVal) const {
    bool success = false;

    if (aVal < iValue) {
        if (iLeft) {
            success = iLeft->Search(aVal);
        }
    } else if (aVal > iValue) {
        if (iRight) {
            success = iRight->Search(aVal);
        }
    } else {
        success = true;
    }

    return success;
}

void BSTNode::Display() const{
    if (iLeft) {
        iLeft->Display();
    }
    cout<<iValue<<"  ";
    if (iRight) {
        iRight->Display();
    }
}

int BSTNode::MinValue() const {
    int val = iValue;
    if (iLeft) {
        val = iLeft->MinValue();
    }
    return val;
}
