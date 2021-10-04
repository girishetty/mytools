#include "..\include\LRUSlot.h"

void LRUSlot::AddEntry(Entry& anEntry) {
  //Both LRU & MRU uses the same logic
  //the Most recently added/referenced/used item will be in the begining of the list
  iEntry.push_front(anEntry);
}

void LRUSlot::RemoveEntry() {
  //LRU removes the least recently used item from the list, which is the last item in the list
  iEntry.pop_back();
}
