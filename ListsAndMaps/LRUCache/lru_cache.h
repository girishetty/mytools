// Provide header for LRU Cache
// 1. Access members in the cache
// 2. Add new members to the cache
// 3. Evict least recently used member if necessary

#pragma once

#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

typedef std::string Key;

struct Item {
public:
  Item(size_t id, const std::string& name, const std::string& info = "") :
    mID(id),
    mName(name),
    mInfo(info) {}

  std::string name() const {
    return mName;
  }

  void display() const {
    std::cout << "ID: [" << mID << "] Name: [" << mName << "] Info: [" << mInfo << "]" << std::endl;
  }

private:
  size_t      mID;
  std::string mName;
  std::string mInfo;
};

// template <class KEY, class VALUE> class LRUCache {
class LRUCache {
private:
  // Use this to allow creation of LRUCache as unique_ptr and
  // restrict the creation of the same by other means.
  struct ForSharedPtr {
    explicit ForSharedPtr(int) {}
  };

public:
  // creates a LRUCache with given capacity
  static std::unique_ptr<LRUCache> Create(size_t capacity) {
    return std::make_unique<LRUCache>(capacity, ForSharedPtr{0});
  }

  LRUCache(size_t capacity, const ForSharedPtr&) : mCapacity(capacity) {}

  // Function to display contents of cache
  void display() const;

  // Get item associated with the given key.
  // On success, it returns the item associated with the key and returns true.
  // If the item is not found, then it returns false.
  bool retrieve(const Key& key, Item& item);

  // Add an item to LRUCache, which will evict the LRU item if its full.
  // Returns true if an item has been evicted; false otherwise.
  bool add(const Key& key, const Item& item);

  // Check an item exists in the Cache.
  // If the item is not found, then it returns false.
  bool exists(const Key& key) const {
    return (mCacheMap.count(key) > 0);
  }

  // Get current size of the LRUCache
  size_t getSize() const {
    return mQueue.size();
  }

private:
  size_t mCapacity;
  std::list<std::pair<Key, Item>> mQueue;
  std::unordered_map<Key, std::list<std::pair<Key, Item>>::iterator> mCacheMap;
};
