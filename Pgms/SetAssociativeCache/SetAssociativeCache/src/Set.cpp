#include "..\include\Slot.h"
#include "..\include\LRUSlot.h"
#include "..\include\MRUSlot.h"
#include "..\include\Set.h"

Set::Set() : iNumSlots(0), iSlots(NULL) {
}

Set::~Set() {
	delete iSlots;
}

void Set::Init(int aSlots, ReplacementPolicy aPolicy) {
	if(aPolicy == LRU) {
		iSlots = new LRUSlot(aSlots);
	}
	//default Policy is MRU
	else {
		iSlots = new MRUSlot(aSlots);
	}
}

void Set::Add(const void* aKey, int aKeySize, const void* aValue, int aValueSize) {
	iSlots->Add(aKey, aKeySize, aValue, aValueSize);
}

void* Set::Lookup(const void* aKey, int aKeySize) {
	return iSlots->Lookup(aKey, aKeySize);
}
