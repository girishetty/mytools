/*
 * https://www.geeksforgeeks.org/print-path-between-any-two-nodes-in-a-binary-tree/
 *
 * Given a Binary Search Tree of distinct nodes and a pair of nodes,
 * The task is to find and print the path between the two given nodes in the binary tree.
 */

#include "BinaryTree.h"

bool pathFromBSTNodeToValue(const TreeNode* node, int value, std::vector<int>& path) {
  if (node == nullptr) {
    // No path
    return false;
  }
  path.push_back(node->val);
  if (value == node->val) {
    // Found the node with the value
    return true;
  }

  if (value < node->val) {
    // Should be in left sub-tree
    return pathFromBSTNodeToValue(node->left, value, path);
  }
  
  // Should be in right sub-tree
  return pathFromBSTNodeToValue(node->right, value, path);
}

bool pathBetweenBSTNodes(const TreeNode* root, int begin, int end, std::vector<int>& path) {
  path.clear();
  if (root == nullptr) {
    // No path!
    return false;
  }

  // Find the Path from root to begin and root to end
  std::vector<int> pathToBegin;
  std::vector<int> pathToEnd;
  if (pathFromBSTNodeToValue(root, begin, pathToBegin) &&
      pathFromBSTNodeToValue(root, end, pathToEnd)) {
    // There exists a path, which can be:
    // 1. In left sub-tree ==> we need to look for the intersection
    // 2. In right sub-tree ==> we need to look for the intersection
    // 3. Between left and right sub-tree (passes through root) ==> root is the intersection
    int intersection = 0;
    for (; intersection < pathToBegin.size() || intersection < pathToEnd.size(); intersection++) {
      if (pathToBegin[intersection] != pathToEnd[intersection]) {
        break;
      }
    }

    // This is where two paths meet
    intersection--;

    // Path is : remaining of pathToBegin is reverse order and remaining of pathToEnd in normal order
    for (int i = pathToBegin.size() - 1; i > intersection; i--) {
      path.push_back(pathToBegin[i]);
    }
    for (int i = intersection; i < pathToEnd.size(); i++) {
      path.push_back(pathToEnd[i]);
    }

    return true;
  }
  
  return false;
}
