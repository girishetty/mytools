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

private:
    BSTNode* iRoot;
};

#endif //_BINARY_SEARCH_TREE_H
