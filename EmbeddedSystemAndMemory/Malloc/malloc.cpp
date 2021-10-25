/*
 *
 * Implement a simple version of malloc and free on an embedded system without heap memory
 * 
 * void* em_malloc(size_t s);
 * void em_free(void* p);
 * 
 * char mem_region[MEM_REGION] = {0};
 * 
 *     / mem_region
 *     +---------------+-----+----+
 *     |               |     |    |
 *     +---------------+-----+----+
 *                   p/<--s-->
 * 
 * Usage:
 *   void* p = em_malloc(s)
 *   // use p
 *   em_free(p)
 * 
 * 
 *  - single threaded
 *  - no need of barrier
 *  - no alloc on size
 *  - no performance req for now
 *  - try to reuse free’d memory
*/


#include <iostream>
#include "malloc.h"

inline int Align(int value, int bytes) {
  int align = (bytes - 1);
  return (((value) + align) & (~align));
}

// Assuming we get 10 MB of memory for heap
static const size_t kMaxHeapMemorySize = 10 * 1024 * 1024;
static uint8_t mem_region[kMaxHeapMemorySize] = {0};

// Data structure to maintain Available and Used memory blocks
typedef struct MemoryBlock {
  size_t size;
  MemoryBlock* pNextBlock = nullptr;
} MemoryBlock;

//Table of available memory blocks with position and the size
MemoryBlock* gAvailbleList = (MemoryBlock*)mem_region;
//Table of consumed memory blocks with position and the size
MemoryBlock* gUsedList = nullptr;

void* em_malloc(size_t size) {
  if (gAvailbleList->size == 0) {
    // Very first time ==> Whole of mem_region is available now.
    gAvailbleList->size = kMaxHeapMemorySize - sizeof(MemoryBlock);
    gAvailbleList->pNextBlock = nullptr;
  }
  MemoryBlock* available = gAvailbleList;
  MemoryBlock* previous = nullptr;
  // Find the first block in Available list, that is big enough for us
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
    // No memory available
    return nullptr;
  }

  // We have to remove this block from Available List and move it to Used List
  // But before that, see if we can preserve memory from this block
  int excess = available->size - (size + sizeof(MemoryBlock));
  if (excess <= 0) {
    // There is nothing to preserve
    if (previous) {
      previous->pNextBlock = available->pNextBlock;
    }
  } else {
    // We can break this available block into two pieces
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

void em_free(void* p) {
  if (p == nullptr || gUsedList == nullptr) {
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
    while (usedBlock != nullptr && thisBlock != usedBlock) {
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
  // TODO: We can improvize here by finding blocks of nodes in available list
  // that are right next to each other and club them into one bigger block
}

// Utility function to display state of the memory
void em_display_stat(const char* what, const MemoryBlock* head) {
  std::cout << what << std::endl;
  for (int count = 1; head != nullptr; count++, head = head->pNextBlock) {
    std::cout << "Block#" << count << "  Size: " << head->size << std::endl;
  }
}

// Utility function to display state of the memory
void em_display_stat() {
  std::cout << "============================================================" << std::endl;
  em_display_stat("Available Memory status", gAvailbleList);
  em_display_stat("Used Memory status", gUsedList);
  std::cout << "============================================================" << std::endl;
}
