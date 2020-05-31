#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

class BSTNode;

class BinarySearchTree {
public:
  BinarySearchTree() : iRoot (NULL) {}
  ~BinarySearchTree();
  bool Add(int aVal);
  bool Remove(int aVal);
  bool Search(int aVal) const;
  int Root() const;

  /**
   * The Diameter (width) of a tree (width) is defined as the number of
   * nodes on the longest path between two end nodes.
   */
  size_t GetDiameter() const;

  /**
   * The Height of a tree defined as the the number of nodes along
   * the longest path from the root node down to the farthest leaf node.
   */
  size_t GetHeight() const;

  // In order traversal ==> left, node, right. BST inorder is the sorted ordering.
  void DisplayInOrder() const;
  // Post order traversal ==> left, right, node
  void DisplayPostOrder() const;
  // Pre order traversal ==> node, left, right
  void DisplayPreOrder() const;
  // Level order traversal.
  void DisplayLevelOrder() const;

  // Display the tree with all possible ways.
  void Display() const {
    DisplayInOrder();
    DisplayPostOrder();
    DisplayPreOrder();
    DisplayLevelOrder();
  }

  /**
   * A simple in-place recursive function to convert the BST into Doubly Linked List.
   * Returns the Pointer to head node of created doubly linked list.
   */
  BSTNode* BinarySearchTreeToDoubleLinkedList();

private:
  size_t maxDown(BSTNode* node, size_t& maxLen) const;
  size_t GetHeight(BSTNode* node) const;
  void DisplayGivenLevel(BSTNode* node, size_t level) const;
  void ConvertToDoubleLinkedList(BSTNode* node, BSTNode* prev, BSTNode*& head);

private:
  BSTNode* iRoot;
};

#endif //_BINARY_SEARCH_TREE_H
