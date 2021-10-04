#ifndef _NODE_H
#define _NODE_H

#include <memory>
#include "Entry.h"

//Collection of childrens
class NodeCollection;

//Represents a tree node. It can be the root Node, which holds no info or any node in the tree
class Node {
public:
  Node(char aKey, const Entry& aEntry);

  ~Node() {}
  
  //Gets the parent node
  Node* Parent() const {
    return iParent;
  }

  void Parent(Node* aParent) {
    iParent = aParent;
  }

  //Gets the children
  std::shared_ptr<NodeCollection> Children() const {
    return iChildren;
  }

  //Gets the root node
  const Node* Root() const;

  //Determins if this node is ancestor of the given node
  bool IsParentOfNode(Node* pNode) const;

  //Determins if this node is descendant of the given node
  bool IsChildOfNode(Node* pNode) const;

  //determines if this node shares hierarchy with given node
  bool IsParentOrChild(Node* pNode) const;

  //Get the corresponding Child Node
  Node* GetChildNodeByKey(char aKey) const;

  bool AddChild(Node* pChild);

  void DisplayDepthNodes() const;

  void DeleteDepthNodes();
  
  const char* Name() const {
    return iEntry->Name();
  }

  const char* PhoneNum() const {
    return iEntry->PhoneNum();
  }

  const std::shared_ptr<Entry> PhoneEntry() const {
    return iEntry;
  }

  //Gets the key associated with the Node
  char Key() const {
    return iKey;
  }

  bool IsTerminal() const {
    return iIsTerminal;
  }

  void IsTerminal(bool val) {
    iIsTerminal = val;
  }

private:
  //value of each node
  char            iKey;
  //is terminal node
  bool            iIsTerminal = false;
  Node*           iParent = nullptr;
  //Children
  std::shared_ptr<NodeCollection> iChildren;
  //Phone Entry with the each node, though not all nodes have valid entry
  std::shared_ptr<Entry>          iEntry;

};

#endif //_NODE_H
