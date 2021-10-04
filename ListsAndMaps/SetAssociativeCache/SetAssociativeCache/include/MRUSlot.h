#ifndef _MRU_SLOT_H
#define _MRU_SLOT_H

#include "..\include\Slot.h"

class MRUSlot : public Slot {
public:
  MRUSlot(int aSlots) : Slot(aSlots) {}

private:
  virtual void AddEntry(Entry& anEntry);
  virtual void RemoveEntry();

private:
  MRUSlot(const MRUSlot& aSlot) : Slot(aSlot.iNumSlots) {}
  MRUSlot& operator=(const MRUSlot& aSlot) {}
};

#endif //_MRU_SLOT_H
