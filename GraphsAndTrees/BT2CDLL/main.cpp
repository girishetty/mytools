/*
 * https://www.geeksforgeeks.org/convert-a-binary-tree-to-a-circular-doubly-link-list/
 *
 * Given a Binary Tree, convert it to a Circular Doubly Linked List (In-Place).  
 *
 * The left and right pointers in nodes are to be used as previous and next pointers respectively in converted Circular Linked List.
 * The order of nodes in the List must be the same as in Inorder for the given Binary Tree.
 * The first node of Inorder traversal must be the head node of the Circular List.
 *
 * BT:
 *                             10
 *                      12            15
 *                  25      30    36
 *
 *  CDLL: 25 <=> 12 <=> 30 <=> 10 <=> 36 <=> 15
 *
 */

#include <iostream>

// To represents a node of a Binary Tree
struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  int data;
  Node(int val) : data(val) {}
};

// Display Circular Double Link List
void displayCDLList(Node* head) {
  std::cout << "Circular Linked List is : [ ";
  Node* itr = head;

  if (head != nullptr) {
    do {
      std::cout << itr->data << " ";
      itr = itr->right;
    } while (head != itr);
  }

  std::cout << "]" << std::endl;
}

// A function that appends rightList at the end of leftList.
Node* concatenateCDLList(Node* leftList, Node* rightList) {
  // Return the other if one of them is empty
  if (leftList == nullptr) {
    return rightList;
  }
  if (rightList == nullptr) {
    return leftList;
  }

  // End of left CDLL
  Node* leftEnd = leftList->left;
  // End of right CDLL
  Node* rightEnd = rightList->left;

  // Connect end of left CDLL to begining of right CDLL
  leftEnd->right = rightList;
  rightList->left = leftEnd;

  // Connect end of right CDLL to begining of left CDLL
  rightEnd->right = leftList;
  leftList->left = rightEnd;

  return leftList;
}

Node* BinaryTreeToCList(Node* root) {
  if (root == nullptr) {
    return root;
  }

  // Recursively get to the left and right subtrees
  Node* left = BinaryTreeToCList(root->left);
  Node* right = BinaryTreeToCList(root->right);

  // Create a CDLL out of single node root, which points to self
  root->left = root->right = root;

  // Now concatenate left with root, followed by right (in order)
  return concatenateCDLList(concatenateCDLList(left, root), right);
}

int main()
{
  Node* root = new Node(10);
  root->left = new Node(12);
  root->right = new Node(15);
  root->left->left = new Node(25);
  root->left->right = new Node(30);
  root->right->left = new Node(36);

  Node* head = BinaryTreeToCList(root);
  displayCDLList(head);

  return 0;
}
