/*
 * https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/
 *
 * Allocate size bytes of uninitialized storage whose alignment is specified by alignment.
 * Alignment is always power of 2.
 *
 * The implementation should be thread-safe
 */

#include <map>
#include <mutex>
#include "AlignedAlloc.h"

struct AlligedAddress {
  void* baseAddress_ = nullptr;
  void* alignedAddress = nullptr;
};

static std::mutex gLock;
static std::map<void*, AlligedAddress> gAlignedAllocList;

void* alignedMalloc(size_t size, size_t align) {
  AlligedAddress ptr;
  ptr.baseAddress_ = (void*) malloc (size + align);
  ptr.alignedAddress = ptr.baseAddress_;
  
  size_t offset = align - ((size_t)ptr.baseAddress_ % align);
  ptr.alignedAddress = ((uint8_t*) ptr.alignedAddress + offset);
  
  {
    std::lock_guard<std::mutex> lock(gLock);
    gAlignedAllocList[ptr.alignedAddress] = ptr;
  }
  
  return ptr.alignedAddress;
}

void alignedFree(void* ptr) {
  if (ptr == nullptr) {
    return;
  }

  std::lock_guard<std::mutex> lock(gLock);
  auto it = gAlignedAllocList.find(ptr);
  if (it != gAlignedAllocList.end()) {
    free(it->second.baseAddress_);
    gAlignedAllocList.erase(it);
  }
}
