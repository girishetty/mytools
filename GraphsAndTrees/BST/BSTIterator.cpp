/*
 * https://leetcode.com/problems/binary-search-tree-iterator/
 * Implement the BSTIterator class that represents an iterator over the in-order traversal of a binary search tree (BST):
 *
 * BSTIterator(TreeNode root) Initializes an object of the BSTIterator class. The root of the BST is given as part of the constructor. The pointer should be initialized to a non-existent number smaller than any element in the BST.
 * boolean hasNext() Returns true if there exists a number in the traversal to the right of the pointer, otherwise returns false.
 * int next() Moves the pointer to the right, then returns the number at the pointer.
 * Notice that by initializing the pointer to a non-existent smallest number, the first call to next() will return the smallest element in the BST.
 * 
 * You may assume that next() calls will always be valid. That is, there will be at least a next number in the in-order traversal when next() is called.
 */

#include <vector>
#include <stack>
#include <iostream>
#include "BST.h"

class BSTIterator {
  std::vector<int> values;
  int position = 0;

#ifdef USE_RECURSIVE
  void initializeIterator(TreeNode* node) {
    if (node == nullptr) {
      return;
    }
    initializeIterator(node->left);
    values.push_back(node->val);
    initializeIterator(node->right);
  }
#endif

  void initializeIteratorFaster(TreeNode* node) {
    std::stack<TreeNode*> stack;

    while (node != nullptr || !stack.empty()) {
      while (node != nullptr) {
        stack.push(node);
        node = node->left;
      }
      node = stack.top();
      stack.pop();
      values.push_back(node->val);
      node = node->right;
    }
  }

public:
  BSTIterator(TreeNode* root) {
    values.push_back(-1);
#ifdef USE_RECURSIVE
    initializeIterator(root);
#else
    initializeIteratorFaster(root);
#endif
  }

  inline int next() {
      return values[++position];
  }

  inline bool hasNext() const {
      return position < values.size() - 1;
  }
};

class QuickBSTIterator {
private:
  std::stack<TreeNode*> st_;
  TreeNode* current_ = nullptr;

public:
  QuickBSTIterator(TreeNode* root) : current_(root) {
    while (current_) {
      st_.push(current_);
      current_ = current_->left;
    }
  }

  inline int next() {
    current_ = st_.top();
    st_.pop();

    int ans = current_->val;

    current_ = current_->right;
    while (current_) {
      st_.push(current_);
      current_ = current_->left;
    }

    return ans;
  }

  inline bool hasNext() const {
    return current_ || !st_.empty();        
  }
};
