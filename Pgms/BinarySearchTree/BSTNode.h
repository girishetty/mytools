#ifndef _BST_NODE_H
#define _BST_NODE_H

class BSTNode {
public:
    BSTNode(int aVal) : iValue(aVal), iLeft(NULL), iRight(NULL) {}

    int Value() const {
        return iValue;
    }
    void Value(int aValue) {
        iValue = aValue;
    }

    BSTNode* Left() const {
        return iLeft;
    }
    void Left(BSTNode* pLeft) {
        iLeft = pLeft;
    }

    BSTNode* Right() const {
        return iRight;
    }
    void Right(BSTNode* pRight) {
        iRight = pRight;
    }
    
    bool Add(int aVal);
    BSTNode* Remove(int aVal, BSTNode* aParent);
    bool Search(int aVal) const;
    void Display() const;
    void Delete();

private:
    int MinValue() const;

private:
    int      iValue;
    BSTNode* iLeft;
    BSTNode* iRight;
};

#endif //_BST_NODE_H
