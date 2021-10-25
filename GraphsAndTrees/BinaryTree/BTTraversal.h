/*
 * Binary Tree Traversal
 *
 * Implement following traversal algorithms
 *   - inorder (left, node, right)
 *   - preorder (node, left, right)
 *   - postorder (left, right, node)
 *   - level order (Breadth First)
 *
 */

#pragma once
#include <memory>
#include "BinaryTree.h"

enum BTTraversalType {
  InOrder,
  PreOrder,
  PostOrder,
  InOrderRec,
  PreOrderRec,
  PostOrderRec,
  LevelOrder,
};

class BTTraversal {

public:
  static std::unique_ptr<BTTraversal> createBTTraversal(BTTraversalType type, const TreeNode* root);

  virtual void traverse() = 0;
  virtual void traverse(std::vector<int>& list) = 0;
};
