#include <iostream>
using namespace std;

#include "BSTNode.h"
#include "BST.h"

BinarySearchTree::~BinarySearchTree(){
    //Delete all the elements in the BST
    if (iRoot) {
        iRoot->Delete();
    }
}

int BinarySearchTree::Root() const {
    int val = INT_MIN;
    if (iRoot) {
        val = iRoot->Value();
    }
    return val;
}

bool BinarySearchTree::Add(int aVal) {
    bool success = true;
    if (iRoot) {
        success = iRoot->Add(aVal);
    } else {
        iRoot = new BSTNode(aVal);
    }
    return success;
}

bool BinarySearchTree::Remove(int aVal) {
    bool success = false;
    if (iRoot) {
        if (aVal == iRoot->Value()){
            //Need to remove root now!
            //Create a temporary Root, and make actual root as left child of this temporary root
            BSTNode tempRoot(0);
            tempRoot.Left(iRoot);
            //Now do delete on the Root
            BSTNode* pRemoveNode = iRoot->Remove(aVal, &tempRoot);
            //Set the Root accordingly
            iRoot = tempRoot.Left();
            if (pRemoveNode) {
                delete pRemoveNode;
                success = true;
            }
        } else {
            BSTNode* pRemoveNode = iRoot->Remove(aVal, NULL);
            if (pRemoveNode) {
                delete pRemoveNode;
                success = true;
            }
        }
    }
    return success;
}

bool BinarySearchTree::Search(int aVal) const {
    bool success = false;
    if (iRoot) {
        success = iRoot->Search(aVal);
    }
    return success;
}

void BinarySearchTree::Display() const {
    cout<<"The Binary Search Tree is : ";
    if (iRoot) {
        iRoot->Display();
        cout<<endl;
    } else {
        cout<<"<empty>"<<endl;
    }
}
