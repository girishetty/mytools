// Provide header for LRU Cache
// 1. Access members in the cache
// 2. Add new members to the cache
// 3. Evict least recently used member if necessary

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

  // Get item associated with the given key.
  // On success, it returns the item associated with the key and returns true.
  // If the item is not found, then it returns false.
  bool retrieve(const Key& key, Item& item) {
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
  bool add(const Key& key, const Item& item) {
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

  // Check an item exists in the Cache.
  // If the item is not found, then it returns false.
  bool exists(const Key& key) const {
    return (mCacheMap.count(key) > 0);
  }

  // Get current size of the LRUCache
  size_t getSize() const {
    return mQueue.size();
  }

  // Function to display contents of cache
  void display() const {
    for (auto it = mQueue.begin(); it != mQueue.end(); it++) {
      it->second.display();
    }

    std::cout << std::endl;
  }

private:
  size_t mCapacity;
  std::list<std::pair<Key, Item>> mQueue;
  std::unordered_map<Key, std::list<std::pair<Key, Item>>::iterator> mCacheMap;
};

int main() {

  auto cache = LRUCache::Create(10);
  Item i0(0, "Girish0");
  Item i1(1, "Girish1");
  Item i2(2, "Girish2");
  Item i3(3, "Girish3");
  Item i4(4, "Girish4");
  Item i5(5, "Girish5");
  Item i6(6, "Girish6");
  Item i7(7, "Girish7");
  Item i8(8, "Girish8");
  Item i9(9, "Girish9");
  Item i10(10, "Girish10");
  Item i11(11, "Girish11");

  cache->add(i0.name(), i0);
  cache->add(i1.name(), i1);
  cache->add(i2.name(), i2);
  cache->add(i3.name(), i3);
  cache->add(i4.name(), i4);
  cache->add(i5.name(), i5);
  cache->add(i6.name(), i6);
  cache->add(i7.name(), i7);
  cache->add(i8.name(), i8);
  cache->add(i9.name(), i9);
  cache->display();

  cache->add(i10.name(), i10);
  cache->add(i11.name(), i11);
  cache->display();

  cache->add(i0.name(), i0);
  cache->add(i6.name(), i6);
  cache->add(i7.name(), i7);
  cache->display();
  return 0;
}
