#include "..\include\Set.h"
#include "..\include\Cache.h"

/*
 * HASH_INDEX_BYTES defines how many byte values are used to generate the hash index.
 * To simplyfy the problem, I am taking first 4 bytes now.
 */
#define HASH_INDEX_BYTES 4
#define nullptr 0

Cache* Cache::iCache = nullptr;
/*
 * Public Initializer for setting up the Cache
 * that takes MaxEntries in cache, numner of slots (N-way) and the Replacement Policy
 */
void Cache::SetupCache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy) {
  if(!iCache) {
    //first time creation
    //ToDo: In a multi-threaded environment, we need to protect this by using a lock
    //iCacheLock.Lock();
    iCache = new Cache(aMaxEntry, aSlots, aPolicy);
    //iCacheLock.UnLock();
  }
}

/*
 * Public Initializer for freeing up the Cache
 */
void Cache::DestroyCache() {
  delete iCache;
  iCache = nullptr;
}

Cache* Cache::GetCache() {
  return iCache;
};


//Constructor that sets up the array based HashTable for Sets
Cache::Cache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy) : 
      iMaxEntry(aMaxEntry), 
      iNumSlots(aSlots), 
      iNumSets(aMaxEntry/aSlots){
  iSets = new Set[iNumSets];
  for(int i=0; i<iNumSets; i++) {
    iSets[i].Init(aSlots, aPolicy);
  }
}

Cache::~Cache() {
  delete [] iSets;
}

//To Add Key/Value pair to the Cache.
void Cache::Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize) {

  //ToDo: In a multi-threaded environment, we need to protect this by using a locl
  //iCacheLock.Lock();
  //Get the hash index for the Set
  unsigned int hashIndex = GetHashIndex((unsigned char*)aKey, aKeySize);

  //With in this Set, Add the Key/Value pair to one of the Slot
  iSets[hashIndex].Add(aKey, aKeySize, aValue, aValueSize);
  //iCacheLock.UnLock();
}

//To retrieve the Value based on the Key from the Cache.
void* Cache::Retrieve(const void* aKey, int aKeySize) {
  //ToDo: In a multi-threaded environment, we need to protect this by using a locl
  //iCacheLock.Lock();
  //Get the hash index for the Set
  unsigned int hashIndex = GetHashIndex((unsigned char*)aKey, aKeySize);
  
  //With in this Set, Look for the Key in all the slots
  return iSets[hashIndex].Lookup(aKey, aKeySize);
  //iCacheLock.UnLock();
}

//A simple algorithm for generating Hash Index
unsigned int Cache::GetHashIndex(unsigned char* aKeyData, int aKeySize) {
  //use MIN_OF(first HASH_INDEX_BYTES or aKeySize) bytes as the hash index.
  unsigned int hashIndex = 0;
  int hashIndexBytes = (aKeySize < HASH_INDEX_BYTES)? aKeySize : HASH_INDEX_BYTES;
  
  //Add the values in each of these bytes
  for(int i=0; i<hashIndexBytes; i++) {
    hashIndex += aKeyData[i];
  }

  //Get the index using number of sets
  hashIndex %= iNumSets;

  return hashIndex;
}

