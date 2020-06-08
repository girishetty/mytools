#ifndef _NODE_COLLECTION_H
#define _NODE_COLLECTION_H

#define NO_OF_ALPHABETS  26

class Node;

class NodeCollection {
public:
  NodeCollection(Node* aOwner);

  int Count() const {
    return iSize;
  }

  Node* Owner() const {
    return iOwner;
  }

  bool Add(Node* pNode);
  void Remove(Node* pNode);
  bool Contains(Node* pNode) const;
  Node* operator[](int index) const;
  Node* Find(char aKey) const;
  Node* GetValidNode(int index);
  Node* GetNextValidNode();
  
  //Expose this so that user can reset it and get the first valid node
  void ResetLastValidNode() {
    iLastAccessedNode = -1;
  }

private:
  //Each Node can have maximum of 26 Nodes (for 26 characters)
  int   iSize = 0;
  int   iLastAccessedNode = -1;
  Node* iOwner = nullptr;
  Node* iList[NO_OF_ALPHABETS] = { nullptr };
};

#endif //_NODE_COLLECTION_H
