#include <iostream>
#include <limits.h>

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
  cout << "Added: (" << aVal << ") Diameter And Height: ("
       << GetDiameter() << ":" << GetHeight() << ")" << endl;
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
      BSTNode* pRemoveNode = iRoot->Remove(aVal, nullptr);
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

void BinarySearchTree::DisplayLevelOrder() const {
  cout<<"The Binary Search Tree in Level-Order is : " << endl;
  if (iRoot) {
    auto height = GetHeight();
    for (size_t level = 1; level <= height; level++) {
      cout<<"Level#" << level << ": ";
      DisplayGivenLevel(iRoot, level);
      cout<<endl;
    }
  } else {
    cout<<"<empty>"<<endl;
  }
}

void BinarySearchTree::DisplayGivenLevel(BSTNode* node, size_t level)const {
  if (node == nullptr) {
    return;
  }
  if (level == 1) {
    cout << node->Value() << " ";
  } else if (level > 1) {
    DisplayGivenLevel(node->Left(), level-1);
    DisplayGivenLevel(node->Right(), level-1);
  }
}

void BinarySearchTree::DisplayPostOrder() const {
  cout<<"The Binary Search Tree in Post-Order is : ";
  if (iRoot) {
    iRoot->DisplayPostOrder();
    cout<<endl;
  } else {
    cout<<"<empty>"<<endl;
  }
}

void BinarySearchTree::DisplayPreOrder() const {
  cout<<"The Binary Search Tree in Pre-Order is : ";
  if (iRoot) {
    iRoot->DisplayPreOrder();
    cout<<endl;
  } else {
    cout<<"<empty>"<<endl;
  }
}

void BinarySearchTree::DisplayInOrder() const {
  cout<<"The Binary Search Tree in In Order is : ";
  if (iRoot) {
    iRoot->Display();
    cout<<endl;
  } else {
    cout<<"<empty>"<<endl;
  }
}

size_t BinarySearchTree::GetDiameter() const {
  size_t maxLen = 0;
  maxDown(iRoot, maxLen);
  return maxLen;
}

size_t BinarySearchTree::maxDown(BSTNode* node, size_t& maxLen) const {
  if (node == nullptr) {
    return 0;
  }
  size_t maxLeft = maxDown(node->Left(), maxLen);
  size_t maxRght = maxDown(node->Right(), maxLen);
  maxLen = std::max(maxLen, maxLeft + maxRght);
  return std::max(maxLeft, maxRght) + 1;
}

size_t BinarySearchTree::GetHeight() const {
  return GetHeight(iRoot);
}

size_t BinarySearchTree::GetHeight(BSTNode* node) const {
  if (node == nullptr) {
    return 0;
  }
  size_t leftHeight = GetHeight(node->Left());
  size_t rightHeight = GetHeight(node->Right());
  return std::max(leftHeight, rightHeight) + 1;
}

BSTNode* BinarySearchTree::BinarySearchTreeToDoubleLinkedList() {
  BSTNode* head = nullptr;
  BSTNode* prev = nullptr;
  ConvertToDoubleLinkedList(iRoot, prev, head);
  return head;
}

void BinarySearchTree::ConvertToDoubleLinkedList(BSTNode* node, BSTNode* prev, BSTNode*& head) {
  // Exit condition.
  if (node == nullptr) {
    return;
  }

  // Recursively convert left subtree.
  ConvertToDoubleLinkedList(node->Left(), prev, head);

  // Now convert this node.
  node->Left(prev);
  if (prev == nullptr) {
    // We have found the head of Doubly-Linked List now.
    head = node;
  } else {
    prev->Right(node);
  }
  prev = node;

  // Finally convert right subtree.
  ConvertToDoubleLinkedList(node->Right(), prev, head);
}

