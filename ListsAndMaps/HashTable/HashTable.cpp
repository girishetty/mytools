#include <iostream>
#include "LinkedList.h"
#include "HashTable.h"

using std::cout;
using std::endl;
using std::string;

//Global creation function
HashTable* HashTable::CreateHashTable(size_t aTableSize) {
  HashTable* pNew = new HashTable(aTableSize);
  if (pNew) {
    pNew = pNew->Construct();
  }

  return pNew;
}

//Default contructor
HashTable::HashTable(size_t aTableSize) : iTableSize(aTableSize), iLength(0) {
}

//Hiding Copy constructor from user
HashTable::HashTable(const HashTable& aTable) : iTableSize(0) {
}

//Hiding assignment operator from user
HashTable& HashTable::operator=(const HashTable& aTable) {
  return *this;
}

//Private helper for constructor
HashTable* HashTable::Construct() {
  HashTable* pTable = this;

  iTable = new LinkedList[iTableSize];
  if (!iTable) {
    //Something went wrong, do self destruction and return nullptr
    delete this;
    pTable = nullptr;
  }

  return pTable;
}

//Public Destructor
HashTable::~HashTable() {
  delete [] iTable;
}

#define USE_DBJ_HASH_FUNCTION

#ifdef USE_DBJ_HASH_FUNCTION
/*
 * This algorithm was first reported by Dan Bernstein many years ago in comp.lang.c.
 *   hash(i) = hash(i - 1) * 33 + str[i];
 * Another version of this algorithm (now favored by Bernstein) uses xor:
 *   hash(i) = hash(i - 1) * 33 ^ str[i];
 * Reference: http://www.cse.yorku.ca/~oz/hash.html
 *
 * We are adding extra logic to wrap the hash-index to be with in HashTable size
 */
const size_t kDJB2Const = 5381;

size_t HashTable::HashIndex(const string& aKey) const {
  size_t    hashIndex = 0;
  unsigned long hash = kDJB2Const;
  int       index = 0;
  int       size = aKey.size();

  for (index = 0; index < size; index++) {
    //hash(i) = hash(i - 1) * 33 ^ str[i]
    hash = ((hash << 5) + hash) ^ aKey[index];
  }

  //As HashTable size is iTableSize, lets make sure the index is in the range.
  hashIndex = (size_t) (hash % iTableSize);
  return hashIndex;
}
#else
/*
 * This algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library.
 * It was found to do well in scrambling bits, causing better distribution of the keys and fewer splits.
 * It also happens to be a good general hashing function with good distribution.
 * The actual function is hash(i) = hash(i - 1) * 65599 + str[i];
 * What is included below is the faster version used in gawk. [there is even a faster, duff-device version]
 * The magic constant 65599 was picked out of thin air while experimenting with different constants,
 * and turns out to be a prime.
 * This is one of the algorithms used in berkeley db (see sleepycat) and elsewhere.
 * Reference: http://www.cse.yorku.ca/~oz/hash.html
 *
 * We are adding extra logic to wrap the hash-index to be with int HashTable size
 */

size_t HashTable::HashIndex(const string& aKey) const {
  size_t    hashIndex = 0;
  unsigned long hash = 0;
  int       index = 0;
  int       size = aKey.size();

  for (index = 0; index < size; index++) {
    //hash(i) = hash(i - 1) * 65599 + str[i]
    hash = (hash << 6) + (hash << 16) - hash + aKey[index];
  }

  //As HashTable size is iTableSize, lets make sure the index is in the range.
  hashIndex = (size_t) (hash % iTableSize);
  return hashIndex;
}
#endif

//Public interfaces for Inserting a Node to the HashTable
//On SUCCESS returns true; false otherwise
bool HashTable::Insert(Node* pNode, bool bOwnIt) {
  bool   bSuccess = false;
  size_t hashIndex = 0;

  if (pNode) {
    hashIndex = HashIndex(pNode->Value());
    bSuccess = iTable[hashIndex].Insert(pNode, bOwnIt);
    if (bSuccess) {
      iLength++;
    }
  }

  return bSuccess;
}

bool HashTable::Insert(const string& aValue) {
  bool   bSuccess = false;
  size_t hashIndex = 0;

  hashIndex = HashIndex(aValue);
  bSuccess = iTable[hashIndex].Insert(aValue);
  if (bSuccess) {
    iLength++;
  }

  return bSuccess;
}

bool HashTable::Insert(const char* pValue) {
  bool   bSuccess = false;
  size_t hashIndex = 0;
  string val(pValue);

  if (pValue) {
    hashIndex = HashIndex(val);
    bSuccess = iTable[hashIndex].Insert(val);
    if (bSuccess) {
      iLength++;
    }
  }

  return bSuccess;
}

//Public interfaces for Removing a Node by its value from the HashTable
//On SUCCESS returns true; false otherwise
bool HashTable::Remove(const string& aValue) {
  bool   bSuccess = false;
  size_t hashIndex = 0;

  hashIndex = HashIndex(aValue);
  bSuccess = iTable[hashIndex].Remove(aValue);
  if (bSuccess) {
    iLength--;
  }

  return bSuccess;
}

bool HashTable::Remove(const char* pValue) {
  bool   bSuccess = false;
  size_t hashIndex = 0;
  string val(pValue);

  if (pValue) {
    hashIndex = HashIndex(val);
    bSuccess = iTable[hashIndex].Remove(val);
    if (bSuccess) {
      iLength--;
    }
  }

  return bSuccess;
}

//Public interfaces for Finding a Node by its value from the HashTable
//On SUCCESS returns the Node; nullptr otherwise
Node* HashTable::Find(const string& aValue) const {
  Node*  pNode = nullptr;
  size_t hashIndex = 0;

  hashIndex = HashIndex(aValue);
  pNode = iTable[hashIndex].Find(aValue);

  return pNode;
}

Node* HashTable::Find(const char* pValue) const {
  Node*  pNode = nullptr;
  size_t hashIndex = 0;
  string val(pValue);

  if (pValue) {
    hashIndex = HashIndex(val);
    pNode = iTable[hashIndex].Find(val);
  }

  return pNode;
}

//Public interface for Printing the HashTable
void HashTable::Print() const {
  int index = 0;

  cout << "HashTable: Total Items (" << iLength << "): {" << endl;
  for (index = 0; index < iTableSize; index++) {
    if (iTable[index].Size() > 0) {
      cout << "HashTable: Bucket [" << index + 1 << "] : ";
      iTable[index].Print();
    }
  }
  cout << "}" << endl;
}

//Public interface for Displaying the Histogram of the HashTable
void HashTable::Histogram() const {
  int  index = 0;
  int  insideIndex = 0;
  size_t listSize = 0;

  cout << "HashTable: Total Items (" << iLength << "): {" << endl;
  for (index = 0; index < iTableSize; index++) {
    listSize = iTable[index].Size();
    if (listSize > 0) {
      cout << "HashTable: Bucket [" << index + 1 << "] : ";
      for (insideIndex = 0; insideIndex < listSize; insideIndex++) {
         cout << " X";
      }
      cout << endl;
    }
  }
  cout << "}" << endl;
}
