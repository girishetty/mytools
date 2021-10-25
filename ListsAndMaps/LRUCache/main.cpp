// Provide header for LRU Cache
// 1. Access members in the cache
// 2. Add new members to the cache
// 3. Evict least recently used member if necessary

#include "lru_cache.h"

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
