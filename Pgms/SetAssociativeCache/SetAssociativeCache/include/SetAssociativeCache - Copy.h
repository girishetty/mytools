#ifndef _SET_ASSOCIATIVE_CACHE_H
#define _SET_ASSOCIATIVE_CACHE_H

#include <Cache.h>

/*
 * template based SetAssociativeCache that can be instantiated using <Key, Value> type pair
 * This implementation is based on thin-template pattern where in the actual implamentation is
 * hidden under the concrete class "Cache" from which SetAssociativeCache is derived from
 * Cache class is part of the "library" and this header is exported for the client use
  */

templace <class Key, class Value> class SetAssociativeCache : public Cache {
public:

	/*
	 * Public constructor for setting up the Cache
	 * that takes MaxEntries in cache, numner of slots (N-way) and the Replacement Policy
	 */
	SetAssociativeCache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy) : Cache(aMaxEntry, aSlots, aPolicy) {}

	/*
	 * Public interface - Add that adds Key-Value pair to the Cache
	 */
	void Add(const Key& aKey, Value& aValue) {
		Add(&aKey, sizeof(Key), &aValue, sizeof(Value));
	}

	/*
	 * Public interface - Retrieve that returns Value corresponding to the Key
	 */
	Value& Retrieve(const Key& aKey) {
		Value& value = *((Value*)Retrive(&aKey, sizeof(Key));
	}

private:
	//Making below functions as private to make them non-copyable
	SetAssociativeCache(const SetAssociativeCache& aCache) {}
	SetAssociativeCache& operator=(const SetAssociativeCache& aCache) {}
};

#endif //_SET_ASSOCIATIVE_CACHE_H
