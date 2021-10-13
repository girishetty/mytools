/*
 * Builds a Binary Tree given a vector of numbers.
 * End of the node (child) is represented by -1
 *
 * Example#2:
 *   Input: {1, 2, 2, 2, -1, 2};
 *
 *          1 
 *       /     \
 *     2         2
 *   /          /
 * 2          2
 *
 * Example#2:
 *  Input: {10, 5, 15, 3, 7, 13, 18, 1, -1, 6}
 *
 *                10 
 *             /       \
 *           5           15 
 *         /  \          / \
 *       3      7     13     18
 *     /      /
 *   1       6
 */

#include <cmath>
#include <queue>
#include <iomanip>
#include <iostream>
#include "BinaryTree.h"

TreeNode* createBinaryTree(const std::vector<int>& list) {
  TreeNode* root = nullptr;

  if (list.size() > 0) {
    std::queue<TreeNode*> q;
    root = new TreeNode(list[0]);
    q.push(root);

    for (size_t i = 1; i < list.size();) {
      TreeNode* node = q.front();
      q.pop();

      // Add left Node
      auto item = list[i++];
      if (item != -1) {
        node->left = new TreeNode(item);
        q.push(node->left);
      }

      // Add right Node
      if (i < list.size()) {
        item = list[i++];
        if (item != -1) {
          node->right = new TreeNode(item);
          q.push(node->right);
        }
      }
    }
  }

  return root;
}

static TreeNode* createBSTNode(const std::vector<int>& list, int begin, int end) {
  if (begin > end) {
    return nullptr;
  }
  int middle = (begin + end ) >> 1;
  TreeNode* node = new TreeNode(list[middle]);
  node->left = createBSTNode(list, begin, middle - 1);
  node->right = createBSTNode(list, middle + 1, end);
  return node;
}

/*
 * https://leetcode.com/explore/interview/card/top-interview-questions-easy/94/trees/631/
 *
 * Given an integer array nums where the elements are sorted in ascending order,
 * convert it to a height-balanced binary search tree.
 * A height-balanced binary tree is a binary tree in which the depth of the two subtrees
 * of every node never differs by more than one.
 *
 * Example#1:
 *    Input: {-10,-3,0,5,9}
 *    Output:
 *           0
 *         /   \
 *      -10      5
 *        \       \
 *         -3       9
 *
 */
TreeNode* createBalancedBST(const std::vector<int>& list) {
  return createBSTNode(list, 0, list.size() - 1);
}

/*
 * Given a Binary Tree, displays its contents
 *
 */
void display(const char* what, const TreeNode* node) {
  std::cout << what << " is:" << std::endl;
  if (node == nullptr) {
    std::cout << "Empty" << std::endl;
    return;
  }

  int diameter = 0;
  int height = getBinaryTreeHeight(node, diameter);
  std::queue<const TreeNode*> queue;
  queue.push(node);

  while (height > 0 && !queue.empty()) {
    int queueSize = queue.size();
    std::string pefixSpacing(std::pow(2, height - 1), ' ');
    std::cout << pefixSpacing;
    std::string betweenSpacing(std::pow(2, height) - 1, ' ');
    for (; queueSize > 0; queueSize--) {
      node = queue.front();
      queue.pop();
      if (node != nullptr) {
        std::cout << std::setw(4) << node->val << betweenSpacing;
        // Push left right of node to queue
        queue.push(node->left);
        queue.push(node->right);
      } else {
        std::cout << " x " + betweenSpacing;
      }
    }
    height--;
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

/*
 * Given a vector, displays its contents
 *
 */
void display(const char* what, const std::vector<int>& array) {
  std::cout << what << ": { ";

  for (auto item : array) {
    std::cout << item << " ";
  }

  std::cout << "}" << std::endl;
}
