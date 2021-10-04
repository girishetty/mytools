#pragma once

#include <set>
#include <queue>

typedef uint8_t AllocationType;

class DefaultAllocator {
 public:
  // Constructs a pool with some Allocations created up front.
  // When the 2nd argument is 0, it constructs an initially empty pool.
  DefaultAllocator(size_t individual_allocation_size,
                   size_t initial_allocation_count = 0);
  ~DefaultAllocator();

  AllocationType* Allocate();
  void Release(AllocationType* allocation);
  size_t GetTotalBytesAllocated() const;
  size_t GetIndividualAllocationLength() const;

 private:
  // Disallow constructors
  DefaultAllocator() = delete;
  DefaultAllocator(const DefaultAllocator& other) = delete;
  DefaultAllocator(DefaultAllocator&& other) = delete;
  // Disallow assignment operators
  DefaultAllocator& operator=(const DefaultAllocator& other) = delete;
  DefaultAllocator& operator=(DefaultAllocator&& other) = delete;
  void CreatePool();

 private:
  const size_t individual_allocation_size_;
  const size_t initial_allocation_count_;
  size_t allocated_count_ = 0;
  std::queue<AllocationType*> available_list_;
  std::set<AllocationType*> consumed_list_;
};
