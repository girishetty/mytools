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

#pragma once

// allocate memory of given size
void* em_malloc(size_t size);

// Free memory
void em_free(void* p);

// Utility function to display state of the memory
void em_display_stat();
