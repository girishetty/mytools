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

#pragma once
#include <memory>
#include "BinaryTree.h"

enum BTIteratorType {
  CopyIterator,
  QuickIterator,
};

class BTIterator {

public:
  static std::unique_ptr<BTIterator> createBTIterator(BTIteratorType type, const TreeNode* root);

  virtual int next()  = 0;
  virtual bool hasNext() const = 0;
};
