#include <map>
#include <iostream>

using namespace std;

struct AlligedAddress {
  void* baseAddress_ = nullptr;
  void* alignedAddress = nullptr;
};

std::mutex gLock;
std::map<void*, AlligedAddress> gAlignedAllocList;

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

int main() {
  return 0;
}
