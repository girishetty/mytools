/*
 * Problem:
 * Given a char pointer to large buffer of memory, write your own version of my_malloc and 
 * my_free without using any system calls.  Make it as robust as possible.  
 * How would you minimize memory fragmentation?
 * Solution:
 *
 */

/* Assuming heap_base_ptr is pointer to large buffer of memory, which is already set */
void* heap_base_ptr;

/* data structures for house keeping allocated memory slots */
/* abstraction for memory slot */
typedef struct memory_slot {
	void*  ptr;
	size_t size;
};

/* alloc_cell_count */
size_t alloc_cell_count;
/* list of allocated cells */
memory_slot* alloc_list;
/* free_cell_count */
size_t free_cell_count;
/* list of free cells */
memory_slot* free_list;

/* Get the smallest slot bigger or equal to "size" from list of free cells
 * if available, return corresponding pointer, else return NULL
 */
void* allocate_cells_from_free_slots(size_t size);

/* Lookup for "ptr" in alloc list.
 * if present, return corresponding size, else return 0
 */
size_t lookup_in_alloc_list(void* ptr);

/* add the pointer and the size to the list
 * list can be either free_list or alloc_list
 */
void add_to_list(memory_slot* list, void* ptr, size_t size);

void* my_malloc(size_t size) {
	/* validate the size, to be positive integer > 0
	 * and then get the smallest slot bigger or equal to "size" from list of free cells
	 * Add that cell to list of allocated cells, [by adding pointer and the size]
	 * Return that pointer
	 */
	void* ptr = NULL;
	if(size>0) {
		ptr = allocate_cells_from_free_slots(size);
		if(ptr!=NULL) {
			add_to_list(alloc_list, ptr, size);
		}
	}
	return ptr;
}

void my_free(void* ptr) {
	/* validate the ptr, to be NON-NULL 
	 * and then lookup for this pointer in list of allocated cells
	 * and add this to the list of free cells [by adding pointer and the size]
	 */
	if(ptr!=NULL) {
		size_t size = lookup_in_alloc_list(ptr);
		if(size > 0) {
			add_to_list(free_list, ptr, size);
		}
	}
}
