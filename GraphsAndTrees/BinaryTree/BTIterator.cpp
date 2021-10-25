/*
 * https://leetcode.com/problems/binary-search-tree-iterator/
 *
 * Implement the BTIterator class that represents an iterator over the in-order traversal of a binary tree (BT):
 *
 * BTIterator(TreeNode root) Initializes an object of the BTIterator class.
 * The root of the BT is given as part of the constructor.
 * The pointer should be initialized to a non-existent number smaller than any element in the BT.
 *
 *  - boolean hasNext() Returns true if there exists a number in the traversal to the right of the pointer,
 *                      otherwise returns false.
 *  - int next() Moves the pointer to the right, then returns the number at the pointer.
 *
 * If this BT is a BST, then Notice that by initializing the pointer to a non-existent smallest number,
 * the first call to next() will return the smallest element in the BST.
 * 
 * You may assume that next() calls will always be valid.
 * That is, there will be at least a next number in the in-order traversal when next() is called.
 *
 */

#include <stack>
#include "BTIterator.h"

class BTCopyIterator : public BTIterator {
  std::vector<int> values;
  int position = 0;

  void initializeIterator(const TreeNode* node) {
    if (node == nullptr) {
      return;
    }
    initializeIterator(node->left);
    values.push_back(node->val);
    initializeIterator(node->right);
  }

  void initializeIteratorFaster(const TreeNode* node) {
    std::stack<const TreeNode*> stack;

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
  BTCopyIterator(const TreeNode* root) {
    values.push_back(-1);
#ifdef USE_RECURSIVE
    initializeIterator(root);
#else
    initializeIteratorFaster(root);
#endif
  }

  int next() override {
    return values[++position];
  }

  bool hasNext() const override {
    return position < values.size() - 1;
  }
};

class QuickBTIterator : public BTIterator {
private:
  std::stack<const TreeNode*> st_;
  const TreeNode* current_ = nullptr;

public:
  QuickBTIterator(const TreeNode* root) : current_(root) {
    while (current_) {
      st_.push(current_);
      current_ = current_->left;
    }
  }

  int next() override {
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

  bool hasNext() const override {
    return current_ || !st_.empty();        
  }
};

std::unique_ptr<BTIterator> BTIterator::createBTIterator(BTIteratorType type, const TreeNode* root) {
  std::unique_ptr<BTIterator> bst = nullptr;
  switch (type) {
  case CopyIterator:
    bst = std::make_unique<BTCopyIterator>(root);
  case QuickIterator:
    bst = std::make_unique<QuickBTIterator>(root);
  }

  return bst;
}
