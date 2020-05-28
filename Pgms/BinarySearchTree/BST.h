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
    void Display() const;
    int Root() const;
    size_t GetDiameter() const;

    // A simple in-place recursive function to convert the BST into Doubly Linked List.
    // Returns the Pointer to head node of created doubly linked list.
    BSTNode* BinarySearchTreeToDoubleLinkedList();

private:
    size_t maxDown(BSTNode* node, size_t& maxLen) const;
    void ConvertToDoubleLinkedList(BSTNode* node, BSTNode* prev, BSTNode*& head);

private:
    BSTNode* iRoot;
};

#endif //_BINARY_SEARCH_TREE_H
