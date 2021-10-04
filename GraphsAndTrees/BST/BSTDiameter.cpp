/*
 * https://www.geeksforgeeks.org/diameter-of-a-binary-tree-in-on-a-new-method/
 * The diameter of a tree is the number of nodes on the longest path between two leaves in the tree.
 */

#include <vector>
#include <stack>
#include <iostream>
#include "BST.h"

int getBSTHeight(TreeNode* root, int& diameter) {
  if (root == nullptr) {
    return 0;
  }

  int lHeight = getBSTHeight(root->left, diameter);
  int rHeight = getBSTHeight(root->right, diameter);

  // Diameter is max of {1 (current node) + left sub-tree height + right sub-tree height }
  diameter = std::max(diameter, 1 + lHeight + rHeight);

  // Height is 1 (current node) + max of {left sub-tree height and right sub-tree height}
  return 1 + std::max(lHeight, rHeight);
}

int getBSTDiameter(TreeNode* root) {
  if (root == nullptr) {
    return 0;
  }

  int diameter = -1;
  getBSTHeight(root, diameter);
  return diameter;
}
