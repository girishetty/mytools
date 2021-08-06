// Provide header for LRU Cache
// 1. Access members in the cache
// 2. Add new members to the cache
// 3. Evict least recently used member if necessary

struct Item;
struct Key;

class LRUCache {
public:
  // creates a LRUCache with given capacity
  static std::shared_ptr<LRUCache> Create(size_t capacity);

  // Get item associated with the given key.
  // On success, it returns the item associated with the key and returns true.
  // If the item is not found, then it returns false.
  bool retrieve(const Key& key, Item& item) {
    bool success = false;
    auto found = mCacheMap.find(key);
    if (found != mCacheMap.end()) {
      item = found.second;

      // Since this is the most recently accessed, delete it from its current position
      // push it to the front of the queue.
      mQueue.erase(found);
      mQueue.push_front(make::pair<Key, Item>(key, item));
      mCacheMap[key] = mQueue.begin();
      success = true;
    }

    return success;
  }
  
  // Add an item to LRUCache, which will evict the LRU item
  // returns true, if an item has been evicted.
  // false otherwise
  bool add(const Key& key, const Item& item) {
    bool evicted = false;
    // Check if its present in the cache first.
    if (mCacheMap.find(key) == mCacheMap.end()) {
      // Check if the Cache is full already, in which case we need to evict the LRU (back) item.
      if (mQueue.size() == mCapacity) {
        auto lru = mQueue.back();
        mCacheMap.erase(lru.first);
        mQueue.pop_back();
        evicted = true;
      }
      // update reference 
      mQueue.push_front(make::pair<Key, Item>(key, item));
      mCacheMap[key] = mQueue.begin();
    } else {
      // Already present in the Cache. So just move it to the front of the list
      retrieve(key, item);
    }

    return evicted;
  }

  // Get current size of the LRUCache
  size_t LRUCache::getSize() const {
    return mQueue.size();
  }

private:
  size_t mCapacity;
  std::list<std::pair<Key, Item>> mQueue;
  std::unordered_map<std::pair<Key, std::list<Item>::iterator> mCacheMap;
};
