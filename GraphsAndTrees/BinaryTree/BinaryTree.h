#pragma once

#include <vector>
#include <iostream>

/*=====================================================================================
 *                               Binary Tree (BT)
 *                               ================
 *
 * A Binary Tree (BT) is a tree data structure in which each node has at most two children,
 * which are referred to as the left child and the right child.
 *
 * From a graph theory perspective, binary (and K-ary) trees may also be interpret as
 * an undirected, rather than a directed graph, an ordered, rooted tree.
 *
 =====================================================================================*/
/* Definition for a binary tree node. */
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

/* Given the root of a binary tree, check whether it is a mirror of itself (i.e., symmetric around its center). */
bool isSymmetric(const TreeNode* root);
bool isSymmetricRecurssive(const TreeNode* root);

/* Computes the diameter of binary tree with given the root. */
int getBinaryTreeDiameter(const TreeNode* root);

/* Find height and diameter of a tree, given the root */
int getBinaryTreeHeight(const TreeNode* root, int& diameter);

/* Find the Maximum path sum in a Binary Tree, given the root */
int findMaxPathSumInBinaryTree(const TreeNode* root);

/* Given a Binary Tree, convert it to a Circular Doubly Linked List (In-Place). */
TreeNode* binaryTreeToCList(TreeNode* root);

/*=====================================================================================
 *                        Binary Search Tree (BST)
 *                        ========================
 *
 * A BST is a Binary Tree with following properties:
 *  - The left subtree of a node contains only nodes with keys less than the node's key.
 *  - The right subtree of a node contains only nodes with keys greater than the node's key.
 *  - Both the left and right subtrees must also be binary search trees.
 *
 =====================================================================================*/

/* Given the root of a binary tree, determine if it is a valid binary search tree (BST). */
bool isValidBST(const TreeNode* root);

/* Find the path from the given node to value in the BST */
bool pathFromBSTNodeToValue(const TreeNode* node, int value, std::vector<int>& path);

/* Find the path between two elements in the BST, given the root of BST */
bool pathBetweenBSTNodes(const TreeNode* root, int begin, int end, std::vector<int>& path);

/* Find the sum of all elements in the BST between low and high */
int rangeSumBST(const TreeNode* root, int low, int high);

// =====================================================================================
// Utility functions
// =====================================================================================

/* Create a Binary Tree given elements in a vector */
TreeNode* createBinaryTree(const std::vector<int>& list);

/* Create a Balanced Binary Search Tree given sorted elements in a vector */
TreeNode* createBalancedBST(const std::vector<int>& list);

/* Construct Binary Tree from Preorder and Inorder Traversal */
TreeNode* buildTree(const std::vector<int>& preorder, const std::vector<int>& inorder);

/* Given a Binary Tree, displays its contents */
void display(const char* what, const TreeNode* node);

/* Given a vector, displays its contents */
void display(const char* what, const std::vector<int>& array);

/* Display Circular Double Link List */
void displayCDLList(const TreeNode* head);
