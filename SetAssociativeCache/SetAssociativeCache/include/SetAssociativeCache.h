#ifndef _SET_ASSOCIATIVE_CACHE_H
#define _SET_ASSOCIATIVE_CACHE_H

#include "..\include\Cache.h"

/*
 * template based SetAssociativeCache that can be instantiated using <Key, Value> type pair
 * This implementation is based on thin-template pattern where in the actual implamentation is
 * hidden under the concrete class "Cache" which is declared in Cache.h and defined as part of the 
 * distrubuted library
 *
 *
 * To use this library, one should call 
 * 1. InitSetAssociativeCache
 * 2. Use the cache for Add/Retrival on any data type
 * 3. in the end call DestroyCache to free it up
 */

/*
 * Public function for setting up the N-way Set Associative Cache
 * that takes MaxEntries in cache, numner of slots (N-way) and the Replacement Policy
 * This function should be called before invoking any of Add/Retrieve functions
 * PARAM:
 * aMaxEntry: MAX number of entries in the Cache, preferably power of 2 (128, 254, 1024...)
 * aSlots: this represents N in N-way Set Associative Cache, preferably power of 2 (128, 254, 1024...)
 * aPolicy: what replacement policy to be used, LRU, MRU...?
 */
__declspec(dllexport) void InitSetAssociativeCache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy);

/*
 * Public function for freeing up the Cache
 * this function needs to be called when we dont need Cache at all
 */
__declspec(dllexport) void DestroyCache();

/*
 * Public interface - Add that adds Key-Value pair to the Cache
 */
template <class Key, class Value> void AddToCache(const Key& aKey, const Value& aValue) {
  //ToDo: This function can throw an exception based on whether GetCache() return NULL or a valid pointer (if Cache was set up properly)
  Cache::GetCache()->Add(&aKey, sizeof(Key), &aValue, sizeof(Value));
}

/*
 * Public interface - Retrieve that returns Value corresponding to the Key
 */
template <class Key, class Value> void RetrieveFromCache(const Key& aKey, Value& aValue) {
  //ToDo: This function can throw an exception based on whether GetCache() return NULL or a valid pointer (if Cache was set up properly)
  aValue = NULL;
  aValue = (Value)(Cache::GetCache()->Retrieve(&aKey, sizeof(Key)));
}

#endif //_SET_ASSOCIATIVE_CACHE_H
