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

#include <iostream>
#include "BinaryTree.h"
#include "BTIterator.h"
#include "BTTraversal.h"

void testBinaryTreeToCList() {
  TreeNode* root = new TreeNode(10);
  root->left = new TreeNode(12);
  root->right = new TreeNode(15);
  root->left->left = new TreeNode(25);
  root->left->right = new TreeNode(30);
  root->right->left = new TreeNode(36);

  display("Tree", root);

  TreeNode* head = binaryTreeToCList(root);
  displayCDLList(head);
}

void testBinaryTreeTraversal(const char* what, BTTraversalType type, const TreeNode* root) {
  auto tvr = BTTraversal::createBTTraversal(type, root);
  std::cout << "Tree Traversal in " << what << " is: ";
  tvr->traverse();
  std::cout << std::endl;
}

void testBinaryTreeIterator(BTIteratorType type, const TreeNode* root) {
  auto itr = BTIterator::createBTIterator(type, root);
  std::cout << "Tree Traversal in Inorder: ";
  while (itr->hasNext()) {
    std::cout << itr->next() << " ";
  }
  std::cout << std::endl;
}

void testBuildBTFromPreAndInorder(const std::vector<int>& preorder, const std::vector<int>& inorder) {
  display("Preorder List", preorder);
  display("Inorder List", inorder);
  TreeNode* root = buildTree(preorder, inorder);
  testBinaryTreeTraversal("InOrder", InOrder, root);
  testBinaryTreeTraversal("PreOrder", PreOrder, root);
}

void testBinaryTree(const TreeNode* root) {
  // #1: Find Height and Diameter of the Binary Tree
  int diameter = getBinaryTreeDiameter(root);
  int height = getBinaryTreeHeight(root, diameter);
  std::cout << "Given Binary Tree Height is: " << height << " And Diameter is: " << diameter << std::endl;

  // #2: Check if the Binary Tree is symmetrical (mirror image) around the root
  bool success = isSymmetric(root);
  if (success) {
    std::cout << "The Tree is Symmetrical" << std::endl;
  } else {
    std::cout << "The Tree is NOT Symmetrical" << std::endl;
  }

  // #3: Check if the Binary Tree is a Binary Search Tree.
  bool bst = isValidBST(root);
  if (bst) {
    std::cout << "The Tree is a BST" << std::endl;
  } else {
    std::cout << "The Tree is NOT a BST" << std::endl;
  }

  // #4: Test the Iterator
  testBinaryTreeIterator(CopyIterator, root);
  testBinaryTreeIterator(QuickIterator, root);

  // #5: Test the Traversal
  testBinaryTreeTraversal("InOrder", InOrder, root);
  testBinaryTreeTraversal("InorderRec", InOrderRec, root);
  testBinaryTreeTraversal("PreOrder", PreOrder, root);
  testBinaryTreeTraversal("PreOrderRec", PreOrder, root);
  testBinaryTreeTraversal("PostOrder", PostOrderRec, root);
  testBinaryTreeTraversal("PostOrderRec", PostOrderRec, root);
  testBinaryTreeTraversal("LevelOrder", LevelOrder, root);
}

void testBinaryTree(const std::vector<int>& list, int low, int high) {
  display("Input Vector", list);
  // #0: Create Binary Tree out of vector
  TreeNode* root = createBinaryTree(list);
  // #1: Create Binary Tree out of vector
  display("Tree", root);

  // #2: Test all properties of the Binary Tree
  testBinaryTree(root);

  // #3: Check the sum of Binary Tree between two nodes/values
  auto sum = rangeSumBST(root, low, high);
  std::cout << "Range [" << low << " .. " << high << "] ==> Sum: " << sum << std::endl;

  // #4: Print all the nodes in the path of Binary Tree between two nodes/values
  std::vector<int> path;
  if (pathBetweenBSTNodes(root, low, high, path)) {
    std::cout << "The Path between [" << low << " .. " << high << "]";
    display("", path);
  }
  std::cout << "==================================================================" << std::endl;
}

void testBST(const std::vector<int>& list) {
  display("Input Vector", list);
  // #0: Create Binary Tree out of vector
  TreeNode* root = createBalancedBST(list);
  // #1: Create Binary Tree out of vector
  display("Tree", root);

  // #3: Check if the Binary Tree is a Binary Search Tree.
  bool bst = isValidBST(root);
  if (bst) {
    std::cout << "The Tree is a BST" << std::endl;
  } else {
    std::cout << "The Tree is NOT a BST" << std::endl;
  }
}

int main() {
  testBinaryTree({1,2,2,3,4,4,3}, 1, 4);
  testBinaryTree({1,2,2,2,-1,2}, 1, 2);
  testBinaryTree({5,4,1,-1,1,-1,4,2,-1,2,-1}, 1, 4);
  testBinaryTree({2,3,3,4,-1,5,4}, 2, 4);
  testBinaryTree({5,1,4,-1,-1,3,6}, 1, 4);
  testBinaryTree({5,2,7,1,4,6,9,-1,-1,3}, 1, 4);

  testBinaryTree({10,5,15,3,7,-1,18}, 6, 13);
  testBinaryTree({10,5,15,3,7,13,18,1,6}, 7, 15);
  testBinaryTree({10,5,15,3,7,13,18,1,-1, 6}, 6, 13);
  testBinaryTree({10,5,15,3,7,13,18,1,-1, 6}, 6, 18);
  testBinaryTree({182,107,257,68,146,221,296,50,89,128,164,203,239,278,314,41,59,80,98,119,137,155,173,194,212,230,248,269,287,305,323,35,47,56,65,74,86,95,104,113,125,134,143,152,161,170,179,188,200,209,218,227,236,245,254,263,275,284,293,302,311,320,329,32,38,44,-1,53,-1,62,-1,71,77,83,-1,92,-1,101,-1,110,116,122,-1,131,-1,140,-1,149,-1,158,-1,167,-1,176,-1,185,191,197,-1,206,-1,215,-1,224,-1,233,-1,242,-1,251,-1,260,266,272,-1,281,-1,290,-1,299,-1,308,-1,317,-1,326}, 86, 224);
  testBinaryTree({15,9,21,7,13,19,23,5,-1,11,-1,17}, 19, 21);

  testBST({-10,-3,0,5,9});
  testBST({1,3});

  testBinaryTreeToCList();

  testBuildBTFromPreAndInorder({1,2,4,5,3,6,7}, {4,2,5,1,6,3,7});
  testBuildBTFromPreAndInorder({3,9,20,15,7}, {9,3,15,20,7});
  return 0;
}
