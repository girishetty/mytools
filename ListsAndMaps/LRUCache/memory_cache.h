// Implement a Cache system in which:
// - Key can be of size 1 byte to 256 bytes
// - Value can be up to 1 MB
// Implement following functions
// - Value get(Key key) ==> returns value or null
// - bool set(Key key, Value value) ==> saves the key/value pair in the Cache
// - bool delete(Key key) ==> removes the entry corresponding to this key from the Cache.

#pragma once

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

typedef std::string Key;

// Forward declaration
struct Value;

class LRUMemCache {
private:
  // Use this to allow creation of LRUMemCache as unique_ptr and
  // restrict the creation of the same by other means.
  struct ForSharedPtr {
    explicit ForSharedPtr(int) {}
  };

public:
  // creates a LRUMemCache with given capacity
  static std::unique_ptr<LRUMemCache> Create(size_t capacity);

  LRUMemCache(size_t capacity, const ForSharedPtr&);

  // Get item associated with the given key.
  // On success, it returns the item associated with the key and returns true.
  // If the item is not found, then it returns false.
  std::vector<uint8_t> get(const Key& key);

  // Add an item to LRUMemCache, which will evict the LRU item if its full.
  // Returns true if an item has been evicted; false otherwise.
  bool set(const Key& key, const std::vector<uint8_t>& item);

  // Remove/Delete the Value associated with the Key from Memory Cache
  bool remove(const Key& key);

  // Check an item exists in the Cache.
  // If the item is not found, then it returns false.
  bool exists(const Key& key) const;

  // Get current size of the LRUMemCache
  size_t getSize() const;

  // Function to display contents of cache
  void display() const;

private:
  size_t mCapacity;
  std::list<std::pair<Key, Value*>> mQueue;
  std::unordered_map<Key, std::list<std::pair<Key, Value*>>::iterator> mCacheMap;
};
