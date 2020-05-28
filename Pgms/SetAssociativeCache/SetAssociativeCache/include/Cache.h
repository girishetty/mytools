#ifndef _CACHE_H
#define _CACHE_H

//FWD declaration
class Set;

//Enumuration for all the Replacement policies implemented
enum ReplacementPolicy{
	LRU,
	MRU
	//extend this by adding more policies here
};

/*
 * Type independent Cache class that can hold any type of key and value.
 * This implements N-way set associative cache by creating (MaxEntry/Slots) sets and 
 * each set can hold Slots number of entries
 */

class Cache {
public:
	/*
	 * Public Initializer for setting up the Cache
	 * that takes MaxEntries in cache, numner of slots (N-way) and the Replacement Policy
	 */
	__declspec(dllexport) static void SetupCache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy);

	/*
	 * Public function for freeing up the Cache
	 */
	__declspec(dllexport) static void DestroyCache();

	/*
	 * Public Initializer for getting Cache pointer
	 */
	__declspec(dllexport) static Cache* GetCache();
	/*
	 * Public interface - Add that adds Key-Value pair to the Cache
	 */
	__declspec(dllexport) void Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize);

	/*
	 * Public interface - Retrieve that returns Value corresponding to the Key
	 */
	__declspec(dllexport) void* Retrieve(const void* aKey, int aKeySize);

private:
	// Private constructor
	Cache(int aMaxEntry, int aSlots, ReplacementPolicy aPolicy);

	//destructor for cleanup
	~Cache();

	//Making below functions as private to make them non-copyable
	Cache(const Cache& aCache) :iMaxEntry(0), iNumSlots(0), iNumSets(0) {}
	Cache& operator=(const Cache& aCache) {}

	//private function to get the hash index
	unsigned int GetHashIndex(unsigned char* aKeyData, int aKeySize);

private:
	const int iMaxEntry;
	const int iNumSlots;
	const int iNumSets;
	//Set that implements the Hash
	Set*  iSets;
	static Cache* iCache;
};

#endif
