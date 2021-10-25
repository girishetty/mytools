// Provide header for LRU Cache
// 1. Access members in the cache
// 2. Add new members to the cache
// 3. Evict least recently used member if necessary

#include "lru_cache.h"

// Function to display contents of cache
void LRUCache::display() const {
  for (auto it = mQueue.begin(); it != mQueue.end(); it++) {
    it->second.display();
  }

  std::cout << std::endl;
}

// Get item associated with the given key.
// On success, it returns the item associated with the key and returns true.
// If the item is not found, then it returns false.
bool LRUCache::retrieve(const Key& key, Item& item) {
  bool success = false;
  auto found = mCacheMap.find(key);
  if (found != mCacheMap.end()) {
    item = found->second->second;
    // Since this is the most recently accessed, delete it from its current position
    // push it to the front of the queue.
    mQueue.erase(found->second);
    mQueue.push_front(std::pair<Key, Item>(key, item));
    mCacheMap[key] = mQueue.begin();
    success = true;
  }

  return success;
}
  
// Add an item to LRUCache, which will evict the LRU item if its full.
// Returns true if an item has been evicted; false otherwise.
bool LRUCache::add(const Key& key, const Item& item) {
  bool evicted = false;
  // Check if its present in the cache first.
  auto found = mCacheMap.find(key);
  if (found != mCacheMap.end()) {
    // Already present in the Cache. Since we are going to
    // push it to the begining of the list, remove it first
    mQueue.erase(found->second);
  } else {
    // Check if the Cache is full already, in which case we need to evict the LRU (back) item.
    if (mQueue.size() == mCapacity) {
      auto lru = mQueue.back();
      mCacheMap.erase(lru.first);
      mQueue.pop_back();
      evicted = true;
    }
  }

  // Push the item to the front of the list and update the reference
  mQueue.push_front(std::pair<Key, Item>(key, item));
  mCacheMap[key] = mQueue.begin();
  return evicted;
}
