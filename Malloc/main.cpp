/*
implement a simple version of malloc

void* malloc(size_t s);
void free(void* p);

char mem_region[MEM_REGION] = {0};

/ mem_region
+---------------+-----+----+
|               |     |    |
+---------------+-----+----+
              p/<--s-->

p = malloc(s)


single threaded
no need of barrier
no alloc on size
no performance req for now
try to reuse free’d memory
*/


#include <cstdint>
#include <cstring>

#define ALIGN(value, bytes) (((value) + bytes) & (~(bytes - 1)))

inline int Align(int value, int bytes) {
  int mask = (bytes - 1);
  if (value & mask) {
    value = (((value) + bytes) & (~mask));
  }
  return value;
}

#define HEAPMEMORY_SIZE 1000

uint8_t mem_region[HEAPMEMORY_SIZE] = {0};

typedef struct MemoryBlock {
  size_t size;
  MemoryBlock* pNextBlock = nullptr;
} MemoryBlock;

//Table of available memory blocks with position and the size
MemoryBlock* gAvailbleList = (MemoryBlock*)mem_region;
//Table of consumed memory blocks with position and the size
MemoryBlock* gUsedList = nullptr;

void* malloc(size_t size) {
  if (gAvailbleList->size == 0) {
    // Very first time ==> Whole of mem_region is available now.
    gAvailbleList->size = HEAPMEMORY_SIZE - sizeof(MemoryBlock);
    gAvailbleList->pNextBlock = nullptr;
  }
  MemoryBlock* available = gAvailbleList;
  MemoryBlock* previous = nullptr;
  do {
    if (available->size < size) {
      // Look at next available block.
      previous = available;
      available = available->pNextBlock;
    } else {
      // We have found a block that is big enough for size
      // TODO: Improvement: Find the smallest block big enough for "size"
      break;
    }
  } while (available != nullptr);

  if (available == nullptr) {
    return nullptr;
  }

  // We have to remove this block to Available List and move it to Used List
  int excess = available->size - size - sizeof(MemoryBlock);
  if (excess <= 0) {
    if (previous) {
      previous->pNextBlock = available->pNextBlock;
    }
  } else {
    available->size = size;
    uint8_t* address = (uint8_t*)available + sizeof(MemoryBlock) + size;
    MemoryBlock* newAvailable = (MemoryBlock*)address;
    newAvailable->size = excess;
    newAvailable->pNextBlock = available->pNextBlock;
    if (previous) {
      previous->pNextBlock = newAvailable;
    }
    available->pNextBlock = newAvailable;
  }

  if (gAvailbleList == available) {
    gAvailbleList = available->pNextBlock;
  }

  // Add it to the top of Used List
  available->pNextBlock = gUsedList;
  gUsedList = available;

  return (uint8_t*)available + sizeof(MemoryBlock);
}

void free(void* p) {
  if (p == nullptr) {
    return;
  }
  uint8_t* address = (uint8_t*)p - sizeof(MemoryBlock);
  MemoryBlock* thisBlock = (MemoryBlock*)address;
  if (thisBlock == gUsedList) {
    gUsedList = thisBlock->pNextBlock;
  } else {
    //Run through gUsedList to see if we have an entry with thisBlock;
    MemoryBlock* usedBlock = gUsedList;
    MemoryBlock* previous = nullptr;
    while (thisBlock != usedBlock) {
      previous = usedBlock;
      usedBlock = usedBlock->pNextBlock;
    }

    // No such entry - nothing to do.
    if (thisBlock != usedBlock) {
      return;
    }

    // Since we are moving this block to Available list, connect the links
    if (previous != nullptr) {
      previous->pNextBlock = usedBlock->pNextBlock;
    }
  }

  // Add it to the top of Available List
  thisBlock->pNextBlock = gAvailbleList;
  gAvailbleList = thisBlock;
}

int main() {
  return 0;
}
