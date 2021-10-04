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
 * Example#2:
 *  Input: root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10
 *  Output: 23
 *  Explanation: Nodes 6, 7, and 10 are in the range [6, 10]. 6 + 7 + 10 = 23.
 */

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include "BST.h"

TreeNode* createBST(const std::vector<int>& list) {
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

      // Add left Node
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

void displayVector(const char* what, const std::vector<int> array) {
  std::cout << what << ": { ";

  for (auto item : array) {
    std::cout << item << " ";
  }

  std::cout << " }" << std::endl;
}

void testRangeSumBST(std::vector<int> list, int low, int high) {
  TreeNode* root = createBST(list);
  auto sum = rangeSumBST(root, low, high);
  std::cout << "Range [" << low << " .. " << high << "] ==> Sum: " << sum << std::endl;
  std::vector<int> path;
  pathBetweenNodes(root, low, high, path);
  displayVector("Path", path);
}

int main() {
  testRangeSumBST({10,5,15,3,7,-1,18}, 6, 13);
  testRangeSumBST({10,5,15,3,7,13,18,1,6}, 7, 15);
  testRangeSumBST({10,5,15,3,7,13,18,1,-1, 6}, 6, 13);
  testRangeSumBST({10,5,15,3,7,13,18,1,-1, 6}, 6, 18);
  testRangeSumBST({182,107,257,68,146,221,296,50,89,128,164,203,239,278,314,41,59,80,98,119,137,155,173,194,212,230,248,269,287,305,323,35,47,56,65,74,86,95,104,113,125,134,143,152,161,170,179,188,200,209,218,227,236,245,254,263,275,284,293,302,311,320,329,32,38,44,-1,53,-1,62,-1,71,77,83,-1,92,-1,101,-1,110,116,122,-1,131,-1,140,-1,149,-1,158,-1,167,-1,176,-1,185,191,197,-1,206,-1,215,-1,224,-1,233,-1,242,-1,251,-1,260,266,272,-1,281,-1,290,-1,299,-1,308,-1,317,-1,326}, 86, 224);
  testRangeSumBST({15,9,21,7,13,19,23,5,-1,11,-1,17}, 19, 21);

  return 0;
}
