#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

/*
 * Linked List implementation
 * This implements Single Linked List for Add/Remove/Find functionalities
 */

#include <string>

//FWD declaration
class LinkedList;

class Node {

//Make LinkedList friend of this class
friend class LinkedList;

public:
  //Public constructors and assignement operator and the destructor
  Node(const std::string& aData, Node* pNext = nullptr);
  Node(const char* pData, Node* pNext = nullptr);
  Node(const Node& aNode);
  Node(const Node* pNode);
  Node& operator=(const Node& aNode);
  ~Node();

  //Public Value retriever
  const std::string& Value() const {
    return iData;
  }

private:
  std::string iData;
  Node*       iNext = nullptr;
};

class LinkedList {

public:
  //Expose only this constructor and destructor
  LinkedList();
  ~LinkedList();

  //Public interfaces for Inserting a Node to the end of the List
  //On SUCCESS returns true; false otherwise
  bool   Insert(Node* pNode, bool bOwnIt = true);
  bool   Insert(const std::string& aData);
  bool   Insert(const char* pData);

  //Public interfaces for Removing a Node by its value from the List
  //On SUCCESS returns true; false otherwise
  bool   Remove(const std::string& aData);
  bool   Remove(const char* pData);

  //Public interfaces for Finding a Node by its value from the List
  //On SUCCESS returns the Node; nullptr otherwise
  Node*  Find(const std::string& aData) const;
  Node*  Find(const char* pData) const;

  //Public retriever for Size of the List
  size_t Size() const {
    return iLength;
  }

  //Public interface for Printing the List
  void   Print() const;

private:
  //Hide the copy constructor and assignement operator
  LinkedList(const LinkedList& aList);
  LinkedList& operator=(const LinkedList& aList);

  //Private function to Locate a Node by its value
  Node* Locate(const std::string& aData, Node** pPrevNode) const;

private:
  Node*  iHead = nullptr;
  size_t iLength = 0;
};

#endif //_LINKED_LIST_H
