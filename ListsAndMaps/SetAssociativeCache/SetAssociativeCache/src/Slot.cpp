#include "..\include\Slot.h"

void Slot::Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize) {
  if(iCount == iNumSlots) {
    //We don't have a free slot in this set now.
    //Look for a slot to be removed based on the page/slot replacement/eviction policy/algorithm
    RemoveEntry();
  }
  //Construct an Entry for this item.
  Data key(aKey, aKeySize);
  Data value(aValue, aValueSize);
  Entry entry(key, value);

  //Add the entry based on the Policy
  AddEntry(entry);
  iCount++;
}

void* Slot::Lookup(const void* aKey, int aKeySize) {
  //As we are not using hash based slots (which would give better performance), do a linear searh for the key/value
  list<Entry>::iterator it;
  void* pValue = nullptr;
  for(it = iEntry.begin(); it != iEntry.end(); it++) {
    const void* key = (*it).Key().GetData();
    if(memcmp(key, aKey, aKeySize) == 0) {
      //we got the matching key
      Entry entry(*it);
      //Now that this Entry is being referenced, we need to reorder
      //1. Remove it from the current location
      //2. Add it back to the list based on the policy
      iEntry.erase(it);
      AddEntry(entry);
      //so set up the Value for the retrieval
      pValue = (void*) entry.Value().GetData();
      break;
    }
  }

  return pValue;
}
