// Implement a Cache system in which:
// - Key can be of size 1 byte to 256 bytes
// - Value can be up to 1 MB
// Implement following functions
// - Value get(Key key) ==> returns value or null
// - bool set(Key key, Value value) ==> saves the key/value pair in the Cache
// - bool delete(Key key) ==> removes the entry corresponding to this key from the Cache.

#include <memory>
#include <iostream>
#include "memory_cache.h"

/*
 * Value can be of range 0 to 1 MB
 * So, to make use of smaller block of memories, the Value can be constructed as linked list,
 * in which each node is stricted to say, 256 bytes
 */

static const size_t kMaxChunkSize = 256;

struct Chunk {
public:
  Chunk(const std::vector<uint8_t>& in_value) {
    Chunk* last = nullptr;
    size_t offset = 0;
    auto beginIndex = in_value.begin();
    auto size = in_value.size();

    while (size != 0) {
      size_t toCopy = std::min(size, kMaxChunkSize);
      Chunk* node = new Chunk;
      if (last != nullptr) {
        last->next = node;
      }
      node->value.insert(node->value.end(), beginIndex + offset, beginIndex + offset + toCopy);
      size = size - toCopy;
      offset = offset + toCopy;
      last = node;
    }
  }

  Chunk() {}

  ~Chunk() {
    Chunk* chunk = next;
    while (next != nullptr) {
      Chunk* toDelete = next;
      next = next->next;
      delete toDelete;
    }
  }

  std::vector<uint8_t> getValue() const {
    std::vector<uint8_t> out_value = value;

    Chunk* chunk = next;
    while (chunk != nullptr) {
      out_value.insert(out_value.end(), chunk->value.begin(), chunk->value.end());
      chunk = chunk->next;
    }

    return out_value;
  }

private:
  std::vector<uint8_t> value;
  Chunk* next = nullptr;
};

struct Value {
public:
  Value() {}

  Value(const std::vector<uint8_t>& in_value) {
    size = in_value.size();
    chunks = std::make_unique<Chunk>(in_value);
  }

  ~Value() {}

  std::vector<uint8_t> getValue() const {
    return chunks->getValue();
  }

  void display() const {
    std::cout << "Value of size: " << size << std::endl;
  }

private:
  std::unique_ptr<Chunk> chunks;
  size_t size = 0;
};

// creates a LRUMemCache with given capacity
std::unique_ptr<LRUMemCache> LRUMemCache::Create(size_t capacity) {
  return std::make_unique<LRUMemCache>(capacity, ForSharedPtr{0});
}

LRUMemCache::LRUMemCache(size_t capacity, const ForSharedPtr&) : mCapacity(capacity) {}

// Function to display contents of cache
void LRUMemCache::display() const {
  for (auto it = mQueue.begin(); it != mQueue.end(); it++) {
    it->second->display();
  }

  std::cout << std::endl;
}

// Get item associated with the given key.
// On success, it returns the item associated with the key and returns true.
// If the item is not found, then it returns false.
std::vector<uint8_t> LRUMemCache::get(const Key& key) {
  std::vector<uint8_t> item;
  auto found = mCacheMap.find(key);
  if (found != mCacheMap.end()) {
    Value* value = found->second->second;
    item = value->getValue();
    // Since this is the most recently accessed, delete it from its current position
    // push it to the front of the queue.
    mQueue.erase(found->second);
    mQueue.push_front(std::pair<Key, Value*>(key, value));
    mCacheMap[key] = mQueue.begin();
  }

  return item;
}
  
// Add an item to LRUMemCache, which will evict the LRU item if its full.
// Returns true if an item has been evicted; false otherwise.
bool LRUMemCache::set(const Key& key, const std::vector<uint8_t>& item) {
  bool evicted = false;
  // Check if its present in the cache first.
  auto found = mCacheMap.find(key);
  if (found != mCacheMap.end()) {
    // Already present in the Cache. Since we are going to
    // push it to the begining of the list, remove it first
    Value* value = found->second->second;
    mQueue.erase(found->second);
    delete value;
  } else {
    // Check if the Cache is full already, in which case we need to evict the LRU (back) item.
    if (mQueue.size() == mCapacity) {
      auto lru = mQueue.back();
      Value* value = lru.second;
      mCacheMap.erase(lru.first);
      mQueue.pop_back();
      delete value;
      evicted = true;
    }
  }

  // Push the item to the front of the list and update the reference
  Value* value = new Value(item);
  mQueue.push_front(std::pair<Key, Value*>(key, value));
  mCacheMap[key] = mQueue.begin();
  return evicted;
}

bool LRUMemCache::remove(const Key& key) {
  // Check if its present in the cache first.
  auto found = mCacheMap.find(key);
  if (found != mCacheMap.end()) {
    // Delete it from the cache
    mQueue.erase(found->second);
    return true;
  }
  return false;
}

// Check an item exists in the Cache.
// If the item is not found, then it returns false.
bool LRUMemCache::exists(const Key& key) const {
  return (mCacheMap.count(key) > 0);
}

// Get current size of the LRUMemCache
size_t LRUMemCache::getSize() const {
  return mQueue.size();
}
