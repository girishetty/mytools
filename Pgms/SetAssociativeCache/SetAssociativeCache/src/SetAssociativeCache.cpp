#include "..\include\SetAssociativeCache.h"

/*
 * Public function for setting up the N-way Set Associative Cache
 * that takes MaxEntries in cache, numner of slots (N-way) and the Replacement Policy
 * This function should be called before invoking any of Add/Retrieve functions
 */
void InitSetAssociativeCache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy) {
	Cache::SetupCache(aMaxEntry, aSlots, aPolicy);
}

/*
 * Public function for freeing up the Cache
 * this function needs to be called when we dont need Cache at all
 */
void DestroyCache() {
	Cache::DestroyCache();
}

