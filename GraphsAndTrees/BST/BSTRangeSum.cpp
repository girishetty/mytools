/*
 * https://leetcode.com/problems/range-sum-of-bst/
 *
 * Given the root node of a binary search tree and two integers low and high, return the sum of values of all nodes with a value in the inclusive range [low, high].
 *
 * Example#2:
 *  Input: root = [10,5,15,3,7,null,18], low = 7, high = 15
 *  Output: 32
 *  Explanation: Nodes 7, 10, and 15 are in the range [7, 15]. 7 + 10 + 15 = 32.
 *
 * Example#2:
 *  Input: root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10
 *  Output: 23
 *  Explanation: Nodes 6, 7, and 10 are in the range [6, 10]. 6 + 7 + 10 = 23.
 */

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include "BST.h"

int nodeToRootSum(TreeNode* node, int value) {
  if (node == nullptr) {
    return 0;
  }
  if (value <= node->val) {
#ifdef _DEBUG
    std::cout << node->val << "+";
#endif
    return node->val + nodeToRootSum(node->left, value) + nodeToRootSum(node->right, value);
  }
  
  return nodeToRootSum(node->right, value);
}

int rootToNodeSum(TreeNode* node, int value) {
  if (node == nullptr) {
    return 0;
  }
  if (value >= node->val) {
#ifdef _DEBUG
    std::cout << node->val << "+";
#endif
    return node->val + rootToNodeSum(node->left, value) + rootToNodeSum(node->right, value);
  }
  return rootToNodeSum(node->left, value);
}

int rangeSumBST(TreeNode* root, int low, int high) {
  if (root == nullptr) {
    return 0;
  }
  if (low == root->val && high == root->val) {
    return low;
  }
  if (low < root->val && high < root->val) {
    return rangeSumBST(root->left, low, high);
  }
  if (low > root->val && high > root->val) {
    return rangeSumBST(root->right, low, high);
  }
  
  int sum = 0;
  bool rootAdded = false;
  if (low <= root->val) {
    sum = root->val + nodeToRootSum(root->left, low);
    std::cout << std::endl;
    rootAdded = true;
  }
  if (high >= root->val) {
    sum += rootToNodeSum(root->right, high);
    std::cout << std::endl;
    if (!rootAdded) {
      sum += root->val;
    }
  }
  
  return sum;
}
