/*
 * https://www.geeksforgeeks.org/convert-a-binary-tree-to-a-circular-doubly-link-list/
 *
 * Given a Binary Tree, convert it to a Circular Doubly Linked List (In-Place).  
 *
 * The left and right pointers in nodes are to be used as previous and next pointers respectively
 * in converted Circular Linked List.
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

#include "BinaryTree.h"

// A function that appends rightList at the end of leftList.
TreeNode* concatenateCDLList(TreeNode* leftList, TreeNode* rightList) {
  // Return the other if one of them is empty
  if (leftList == nullptr) {
    return rightList;
  }
  if (rightList == nullptr) {
    return leftList;
  }

  // End of left CDLL
  TreeNode* leftEnd = leftList->left;
  // End of right CDLL
  TreeNode* rightEnd = rightList->left;

  // Connect end of left CDLL to begining of right CDLL
  leftEnd->right = rightList;
  rightList->left = leftEnd;

  // Connect end of right CDLL to begining of left CDLL
  rightEnd->right = leftList;
  leftList->left = rightEnd;

  return leftList;
}

TreeNode* binaryTreeToCList(TreeNode* root) {
  if (root == nullptr) {
    return root;
  }

  // Recursively get to the left and right subtrees
  TreeNode* left = binaryTreeToCList(root->left);
  TreeNode* right = binaryTreeToCList(root->right);

  // Create a CDLL out of single node root, which points to self
  root->left = root->right = root;

  // Now concatenate left with root, followed by right (in order)
  return concatenateCDLList(concatenateCDLList(left, root), right);
}
