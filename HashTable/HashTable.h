#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

/*
 * Hash Table implementation, that uses Dan Bernstein's method of Hash generator
 * This also takes of collision detection using Linked List
 */

#include <string>

const size_t kDefaultHashTableSize = 1024;

class LinkedList;

class HashTable {

public:
  //Public interface for creating the HashTable with the given size
  static HashTable* CreateHashTable(size_t aTableSize = kDefaultHashTableSize);

  //Destructor for the HashTable
  ~HashTable();

  //Public interfaces for Inserting a Node to the HashTable
  //On SUCCESS returns true; false otherwise
  bool   Insert(Node* pNode, bool bOwnIt = true);
  bool   Insert(const std::string& aValue);
  bool   Insert(const char* pValue);

  //Public interfaces for Removing a Node by its value from the HashTable
  //On SUCCESS returns true; false otherwise
  bool   Remove(const std::string& aValue);
  bool   Remove(const char* pValue);

  //Public interfaces for Finding a Node by its value from the HashTable
  //On SUCCESS returns the Node; nullptr otherwise
  Node*  Find(const std::string& aValue) const;
  Node*  Find(const char* pValue) const;

  //Public retriever for Size of the HashTable
  size_t Size() const {
    return iTableSize;
  }

  //Public retriever for number of items in the HashTable
  size_t Count() const {
    return iLength;
  }

  //Public interface for Printing the HashTable
  void   Print() const;
  void   Histogram() const;

private:
  //Hide the constructors and the assignment operator
  HashTable(size_t aTableSize);
  HashTable(const HashTable& aTable);
  HashTable& operator=(const HashTable& aTable);

  //Internal constructor for HashTable
  HashTable* Construct();

  //Internal function that generates hash index
  size_t HashIndex(const std::string& aKey) const;

private:
  const size_t iTableSize;
  size_t       iLength = 0;
  LinkedList*  iTable = nullptr;
};

#endif //_HASH_TABLE_H
