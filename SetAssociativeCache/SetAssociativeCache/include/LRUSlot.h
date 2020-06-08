#ifndef _LRU_SLOT_H
#define _LRU_SLOT_H

#include "..\include\Slot.h"

class LRUSlot : public Slot {
public:
  LRUSlot(int aSlots) : Slot(aSlots) {}

private:
  virtual void AddEntry(Entry& anEntry);
  virtual void RemoveEntry();

private:
  LRUSlot(const LRUSlot& aSlot) : Slot(aSlot.iNumSlots) {}
  LRUSlot& operator=(const LRUSlot& aSlot) {}
};

#endif //_LRU_SLOT_H
