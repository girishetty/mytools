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

#include <map>
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
 * https://leetcode.com/explore/interview/card/top-interview-questions-medium/108/trees-and-graphs/788/
 *
 * Construct Binary Tree from Preorder and Inorder Traversal
 * Given two integer arrays preorder and inorder where preorder is the preorder traversal of
 * a binary tree and inorder is the inorder traversal of the same tree, construct and return the binary tree.
 *
 * Constraints:
 *    - 1 <= preorder.length <= 3000
 *    - inorder.length == preorder.length
 *    - -3000 <= preorder[i], inorder[i] <= 3000
 *    - preorder and inorder consist of unique values.
 *    - Each value of inorder also appears in preorder.
 *    - preorder is guaranteed to be the preorder traversal of the tree.
 *    - inorder is guaranteed to be the inorder traversal of the tree.
 *
 * Example#1:
 *    Input: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
 *    Output: [3,9,20,null,null,15,7]
 *
 */

std::map<int, int> createPositionMap(const std::vector<int>& list) {
  std::map<int, int> positionMap;
  for (int i = 0; i < list.size(); i++) {
    positionMap[list[i]] = i;
  }

  return std::move(positionMap);
}

TreeNode* buildTree(const std::vector<int>& preorder, std::map<int, int>& positionMap,
                    int& index, int start, int end) {
  if (start > end) {
      return nullptr;
  }

  auto val = preorder[index++];
  TreeNode* node = new TreeNode(val);

  if (start == end) {
    return node;
  }

  auto position = positionMap[val];
  node->left = buildTree(preorder, positionMap, index, start, position - 1);
  node->right = buildTree(preorder, positionMap, index, position + 1, end);

  return node;
}

TreeNode* buildTree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
  int size = preorder.size();

  if (size == 0) {
      return nullptr;
  }

  std::map<int, int> positionMap = createPositionMap(inorder);
  int index = 0;
  return buildTree(preorder, positionMap, index, 0, size - 1);
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

// Display Circular Double Link List
void displayCDLList(const TreeNode* head) {
  std::cout << "Circular Linked List is : [ ";
  const TreeNode* itr = head;

  if (head != nullptr) {
    do {
      std::cout << itr->val << " ";
      itr = itr->right;
    } while (head != itr);
  }

  std::cout << "]" << std::endl;
}
