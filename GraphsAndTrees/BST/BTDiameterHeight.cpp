/*
 * https://www.geeksforgeeks.org/diameter-of-a-binary-tree-in-on-a-new-method/
 *
 * The diameter of a tree is the number of nodes on the longest path between two leaves in the tree.
 *
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/94/trees/555/
 *
 * Given the root of a binary tree, return its maximum depth.
 *
 * A binary tree's maximum depth/height is the number of nodes along the longest path from the root node down to the farthest leaf node.
 *
 * Example#1:
 *    Input: root = [3,9,20,-1,-1,15,7]
 *    Height: 3 Diameter: 4
 *
 * Example#2:
 *    Input: root = [1,-1,2]
 *    Height: 2 Diameter: 2
 *
 * Example#3:
 *    Input: root = []
 *    Height: 0 Diameter: 0
 *
 */

#include <iostream>
#include "BinaryTree.h"

int getBinaryTreeHeight(const TreeNode* node, int& diameter) {
  if (node == nullptr) {
    return 0;
  }

  int lHeight = getBinaryTreeHeight(node->left, diameter);
  int rHeight = getBinaryTreeHeight(node->right, diameter);

  // Diameter is max of {1 (current node) + left sub-tree height + right sub-tree height }
  diameter = std::max(diameter, 1 + lHeight + rHeight);

  // Height is 1 (current node) + max of {left sub-tree height and right sub-tree height}
  return 1 + std::max(lHeight, rHeight);
}

int getBinaryTreeDiameter(const TreeNode* root) {
  if (root == nullptr) {
    return 0;
  }

  int diameter = -1;
  getBinaryTreeHeight(root, diameter);
  return diameter;
}
