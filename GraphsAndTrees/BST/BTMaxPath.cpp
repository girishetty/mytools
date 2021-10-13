/*
 * https://www.geeksforgeeks.org/find-maximum-path-sum-in-a-binary-tree/
 *
 * Given a binary tree, find the maximum path sum. The path may start and end at any node in the tree.
 *
 * For each node there can be four ways that the max path goes through the node: 
 *  - 1. Node only 
 *  - 2. Max path through Left Child + Node 
 *  - 3. Max path through Right Child + Node 
 *  - 4. Max path through Node + Max path under this Node
 */

#include <limits>
#include <iostream>
#include "BinaryTree.h"

static int findMaxPathSum(const TreeNode* node, int& sum) {
  if (node == nullptr) {
    return 0;
  }

  // Maximum path sum through left child
  int lSum = findMaxPathSum(node->left, sum);
  // Maximum path sum through right child
  int rSum = findMaxPathSum(node->right, sum);

  // Maximum path for this node including one of the child
  int maxNode = std::max(std::max(lSum, rSum) + node->val, node->val);

  // Maximum path for this node - includes both the child path
  int maxPathSumFromNode = std::max(maxNode, lSum + rSum + node->val);

  return maxNode;
}

int findMaxPathSumInBinaryTree(const TreeNode* root) {
  int sum = std::numeric_limits<int>::min();

  findMaxPathSum(root, sum);

  return sum;
}
