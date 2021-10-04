#pragma once

#include <vector>
#include <iostream>

/* Definition for a binary tree node. */
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

/* Create a BST given elements in a vector */
TreeNode* createBST(const std::vector<int>& list);

/* Find the sum of all elements in the BST between low and high */
int rangeSumBST(TreeNode* root, int low, int high);

/* Computes the diameter of binary tree with given the root. */
int getBSTDiameter(TreeNode* root);

/* Find height and diameter of a tree, given the root */
int getBSTHeight(TreeNode* root, int& diameter);

/* Find the path between two elements in the BST */
bool pathBetweenNodes(TreeNode* root, int begin, int end, std::vector<int>& path);

/* Find the Maximum path sum in a Binary Tree, given the root */
int findMaxPathSum(TreeNode* root);

