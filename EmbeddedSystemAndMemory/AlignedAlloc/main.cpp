#include <iostream>
#include "AlignedAlloc.h"

int main() {
  void* ptr1 = alignedMalloc(10, 4);
  void* ptr2 = alignedMalloc(20, 2);
  void* ptr3 = alignedMalloc(10, 8);
  void* ptr4 = alignedMalloc(100, 16);

  std::cout << "Ptr1: " << std::hex << ptr1 << std::endl;
  std::cout << "Ptr2: " << std::hex << ptr2 << std::endl;
  std::cout << "Ptr3: " << std::hex << ptr3 << std::endl;
  std::cout << "Ptr4: " << std::hex << ptr4 << std::endl;

  alignedFree(ptr1);
  alignedFree(ptr2);
  alignedFree(ptr3);
  alignedFree(ptr4);
  return 0;
}
