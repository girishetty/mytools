/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include <iostream>
#include "DefaultAllocator.h"

void DefaultAllocator::CreatePool() {
  for (size_t index = 0; index < initial_allocation_count_; index++) {
    available_list_.push(new AllocationType[individual_allocation_size_]);
  }
  allocated_count_ = initial_allocation_count_;
}

DefaultAllocator::DefaultAllocator(size_t individual_allocation_size,
                                   size_t initial_allocation_count)
    : individual_allocation_size_(individual_allocation_size),
      initial_allocation_count_(initial_allocation_count) {
  if (initial_allocation_count > 0) {
    // Allocate initial_allocation_count no of pool up front.
    CreatePool();
  }
  std::cout << "DefaultAllocator::DefaultAllocator(): Created with " <<
               "initial_allocation_count_: " << initial_allocation_count_ << std::endl;
}

DefaultAllocator::~DefaultAllocator() {
  // Clean up everything in available_list_
  size_t count = 0;
  while (!available_list_.empty()) {
    auto allocation = available_list_.front();
    available_list_.pop();
    delete allocation;
    count++;
  }

  // Clean up everything in consumed_list_
  for (auto item : consumed_list_) {
    delete item;
    count++;
  }
  std::cout << "DefaultAllocator::~DefaultAllocator(): Freed count: " << count <<
               " : initial_allocation_count_: " << initial_allocation_count_ << std::endl;
}

AllocationType* DefaultAllocator::Allocate() {
  AllocationType* allocation = nullptr;

  if (!available_list_.empty()) {
    // Remove the slot from available_list_ and insert it consumed_list_
    allocation = available_list_.front();
    available_list_.pop();
    consumed_list_.insert(allocation);
    std::cout << __func__ << ": Allocated from the pool: " << available_list_.size()
              << " : " << consumed_list_.size() << std::endl;
  } else {
    // Nothing available in the available_list_. So create a new buffer
    allocated_count_++;
    allocation = new AllocationType[individual_allocation_size_];
    std::cout << __func__ << ": Allocated from memory: " << allocated_count_ << std::endl;
  }

  return allocation;
}

void DefaultAllocator::Release(AllocationType* allocation) {
  auto search = consumed_list_.find(allocation);
  if (search != consumed_list_.end()) {
    // Push it back to available_list_ and remove it from consumed_list_
    available_list_.push(allocation);
    consumed_list_.erase(search);
    std::cout << __func__ << ": Released back to the pool: " << available_list_.size()
              << " : " << consumed_list_.size() << std::endl;
  } else {
    allocated_count_--;
    delete allocation;
    std::cout << __func__ << ": Release to the memory: " << allocated_count_ << std::endl;
  }
}

size_t DefaultAllocator::GetTotalBytesAllocated() const {
  return allocated_count_ * individual_allocation_size_ *
         sizeof(AllocationType);
}

size_t DefaultAllocator::GetIndividualAllocationLength() const {
  return individual_allocation_size_;
}

void testAllocator(std::shared_ptr<DefaultAllocator> allocator) {
  std::vector<AllocationType*> list;

  for (size_t index = 0; index < 20; index++) {
    list.push_back(allocator->Allocate());
  }

  std::cout << "TotalBytesAllocated: " << allocator->GetTotalBytesAllocated() << std::endl;

  for (auto item : list) {
    allocator->Release(item);
  }
  list.clear();
}

static size_t kAllocatorSize = 262144; // 256 KB

int main() {
  auto allocator1 = std::make_shared<DefaultAllocator>(kAllocatorSize);
  auto allocator2 = std::make_shared<DefaultAllocator>(kAllocatorSize, 10);

  testAllocator(allocator1);
  testAllocator(allocator2);
  return 0;
}
