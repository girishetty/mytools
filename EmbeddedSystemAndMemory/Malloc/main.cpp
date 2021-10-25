/*
 *
 * Implement a simple version of malloc and free on an embedded system without heap memory
 * 
 * void* em_malloc(size_t s);
 * void em_free(void* p);
 * 
 * char mem_region[MEM_REGION] = {0};
 * 
 *     / mem_region
 *     +---------------+-----+----+
 *     |               |     |    |
 *     +---------------+-----+----+
 *                   p/<--s-->
 * 
 * Usage:
 *   void* p = em_malloc(s)
 *   // use p
 *   em_free(p)
 * 
 * 
 *  - single threaded
 *  - no need of barrier
 *  - no alloc on size
 *  - no performance req for now
 *  - try to reuse free’d memory
*/


#include <iostream>
#include "malloc.h"

int main() {
  void* ptr1 = em_malloc(100);
  void* ptr2 = em_malloc(100);
  void* ptr3 = em_malloc(100);
  void* ptr4 = em_malloc(100);
  void* ptr5 = em_malloc(100);
  em_display_stat();
  em_free(ptr2);
  void* ptr6 = em_malloc(100);
  em_free(ptr1);
  em_free(ptr6);
  void* ptr7 = em_malloc(100);
  em_display_stat();

  em_free(ptr1);
  em_free(ptr2);
  em_free(ptr3);
  em_free(ptr4);
  em_free(ptr5);
  em_free(ptr6);
  em_free(ptr7);
  em_display_stat();
  return 0;
}
