/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/94/trees/627/
 *
 * Given the root of a binary tree, check whether it is a mirror of itself (i.e., symmetric around its center)
 *
 * Example#1:
 *    Input: Input: {1,2,2,3,4,4,3}
 *    Output: true
 *
 * Example#2:
 *    Input: Input: {1,2,2,-1,3,-1,3}
 *    Output: false
 *
 */

#include <stack>
#include <iostream>
#include "BinaryTree.h"

std::vector<int> getPath(const TreeNode* node) {
  std::vector<int> path;
  std::stack<const TreeNode*> stack;

  while (node != nullptr || !stack.empty()) {
    while (node != nullptr) {
      stack.push(node);
      node = node->left;
    }
    node = stack.top();
    stack.pop();
    // Node Value
    path.push_back(node->val);
    node = node->right;
  }

  return path;
}

bool isSymmetric(const TreeNode* root) {
  if (root == nullptr) {
      return true;
  }
  std::stack<const TreeNode*> leftPath;
  std::stack<const TreeNode*> rightPath;
  const TreeNode* leftTree = root->left;
  const TreeNode* rightTree = root->right;

  while (leftTree != nullptr || !leftPath.empty()) {
    while (leftTree != nullptr) {
      if (rightTree == nullptr || rightTree->val != leftTree->val) {
        return false;
      }
      leftPath.push(leftTree);
      leftTree = leftTree->left;
      rightPath.push(rightTree);
      rightTree = rightTree->right;
    }
    if (leftTree != nullptr || rightTree != nullptr) {
      return false;
    }
    leftTree = leftPath.top();
    leftPath.pop();
    leftTree = leftTree->right;

    rightTree = rightPath.top();
    rightPath.pop();
    rightTree = rightTree->left;
  }

  return rightTree == nullptr;
}

bool checkSymmetry(const TreeNode* left, const TreeNode* right) {
  if (left == nullptr && right == nullptr) {
    return true;
  }
  if (left == nullptr || right == nullptr) {
    return false;
  }

  if (left->val != right->val) {
    return false;
  }

  return checkSymmetry(left->left, right->right) && checkSymmetry(left->right, right->left);
}

bool isSymmetricRecurssive(const TreeNode* root) {
  if (root == nullptr) {
      return true;
  }

  return checkSymmetry(root->left, root->right);
}
