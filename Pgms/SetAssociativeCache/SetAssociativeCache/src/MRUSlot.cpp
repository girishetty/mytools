#include "..\include\MRUSlot.h"

void MRUSlot::AddEntry(Entry& anEntry) {
	//Both LRU & MRU uses the same logic
	//the Most recently added/referenced/used item will be in the beginning of the list
	iEntry.push_front(anEntry);
}

void MRUSlot::RemoveEntry() {
	//MRU replaces the most recently added/referenced/used item from the list, which is in the beginning of the list
	iEntry.pop_front();
}
