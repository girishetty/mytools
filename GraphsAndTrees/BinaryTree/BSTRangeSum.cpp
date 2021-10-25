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
 * Example#1:
 *  Input: root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10
 *  Output: 23
 *  Explanation: Nodes 6, 7, and 10 are in the range [6, 10]. 6 + 7 + 10 = 23.
 */

#include "BinaryTree.h"

static int nodeToRootSum(const TreeNode* node, int value) {
  if (node == nullptr) {
    return 0;
  }
  if (value <= node->val) {
    return node->val + nodeToRootSum(node->left, value) + nodeToRootSum(node->right, value);
  }
  
  return nodeToRootSum(node->right, value);
}

static int rootToNodeSum(const TreeNode* node, int value) {
  if (node == nullptr) {
    return 0;
  }
  if (value >= node->val) {
    return node->val + rootToNodeSum(node->left, value) + rootToNodeSum(node->right, value);
  }
  return rootToNodeSum(node->left, value);
}

int rangeSumBST(const TreeNode* root, int low, int high) {
  // Invalid root
  if (root == nullptr) {
    return 0;
  }
  // Range is at the root
  if (low == root->val && high == root->val) {
    return low;
  }

  if (low < root->val && high < root->val) {
    // both low-high are under left sub-tree
    return rangeSumBST(root->left, low, high);
  }
  if (low > root->val && high > root->val) {
    // both low-high are under right sub-tree
    return rangeSumBST(root->right, low, high);
  }
  
  // low is in left sub-tree and high is at right sub-tree
  int sum = 0;
  bool rootAdded = false;
  if (low <= root->val) {
    sum = root->val + nodeToRootSum(root->left, low);
    rootAdded = true;
  }
  if (high >= root->val) {
    sum += rootToNodeSum(root->right, high);
    if (!rootAdded) {
      sum += root->val;
    }
  }
  
  return sum;
}
