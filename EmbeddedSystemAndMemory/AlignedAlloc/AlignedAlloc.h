/*
 * https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/
 *
 * Allocate size bytes of uninitialized storage whose alignment is specified by alignment.
 * Alignment is always power of 2.
 *
 * The implementation should be thread-safe
 */

#pragma once

/*
 * Allocate size bytes of uninitialized storage whose alignment is specified by alignment.
 */
void* alignedMalloc(size_t size, size_t align);

/*
 * Free the memory allocated by alignedMalloc
 */
void alignedFree(void* ptr);
