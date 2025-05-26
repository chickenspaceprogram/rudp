// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <string.h>
#include "minheap.h"

static size_t MINHEAP_INIT_SIZE = 16;

static inline size_t lchild(size_t cur_index)
{
	return cur_index * 2 + 1;
}

static inline size_t rchild(size_t cur_index)
{
	return cur_index * 2 + 2;
}

static inline size_t parent(size_t cur_index)
{
	return (cur_index - 1) / 2;
}

static inline char *getval(struct rudp_min_heap *heap, size_t i)
{
	return heap->elements + (i * heap->elem_size);
}

static inline size_t small_child(struct rudp_min_heap *heap, size_t i)
{
	if (rchild(i) > heap->current_nel) {
		return lchild(i);
	}
	return heap->cmp(getval(heap, lchild(i)), getval(heap, rchild(i))) > 0 ? rchild(i) : lchild(i);
}

static inline bool bad_invariants(struct rudp_min_heap *heap, char *item, size_t i)
{
	if (lchild(i) > heap->current_nel) {
		return false; // both children are nil
	}
	if (rchild(i) > heap->current_nel) {
		if (heap->cmp(item, getval(heap, lchild(i))) > 0) {
			return true;
		}
		return false;
	}
	return heap->cmp(item, getval(heap, lchild(i) > 0)) || heap->cmp(item, getval(heap, rchild(i) > 0));
}

int rudp_min_heap_new(struct rudp_min_heap *heap, struct rudp_allocator *alloc, size_t element_size, int (*cmp)(void *, void *))
{
	if (cmp == NULL || heap == NULL)
		return -RUDP_MIN_HEAP_EINVAL;
	heap->elements = rudp_allocator_alloc(MINHEAP_INIT_SIZE * element_size, alloc);
	if (heap->elements == NULL)
		return -RUDP_MIN_HEAP_EMEM;
	heap->elem_size = element_size;
	heap->max_nel = MINHEAP_INIT_SIZE;
	heap->current_nel = 0;
	heap->cmp = cmp;
	heap->alloc = alloc;
	return 0;
}

void rudp_min_heap_delete(struct rudp_min_heap *heap)
{
	if (heap == NULL)
		return;
	rudp_allocator_free(heap->elements, heap->max_nel * heap->elem_size, heap->alloc);
}


void *rudp_min_heap_peek(const struct rudp_min_heap *heap)
{
	if (heap == NULL)
		return NULL;
	if (heap->current_nel == 0)
		return NULL;
	return heap->elements;
}

int rudp_min_heap_insert(struct rudp_min_heap *heap, void *item)
{
	if (heap == NULL || item == NULL) {
		return -RUDP_MIN_HEAP_EINVAL;
	}
	if (heap->current_nel == heap->max_nel) {
		char *mem = rudp_allocator_reallocarray(heap->elements, heap->max_nel * 2, heap->max_nel, heap->elem_size, heap->alloc);
		if (mem == NULL) {
			return -RUDP_MIN_HEAP_EMEM;
		}
		heap->elements = mem;
	}
	// heap now is def big enough
	if ((heap->current_nel)++ == 0) {
		memcpy(heap->elements, item, heap->elem_size);
		return 0;
	}
	for (size_t i = heap->current_nel - 1; i > 0; i = parent(i)) {
		if (heap->cmp(item, getval(heap, parent(i))) >= 0) { // item is bigger than parent
			memcpy(getval(heap, i), item, heap->elem_size);
			return 0;
		}
		memcpy(getval(heap, i), getval(heap, parent(i)), heap->elem_size);
		if (parent(i) == 0) {
			memcpy(heap->elements, item, heap->elem_size);
		}
	}
	return 0;
}

int rudp_min_heap_pop(struct rudp_min_heap *heap)
{
	if (heap == NULL)
		return -RUDP_MIN_HEAP_EINVAL;
	if (heap->current_nel == 0)
		return -RUDP_MIN_HEAP_EEMPTY;
	
	return rudp_min_heap_replace(heap, getval(heap, --(heap->current_nel)));
}

int rudp_min_heap_replace(struct rudp_min_heap *heap, void *elem)
{
	if (heap == NULL || elem == NULL)
		return -RUDP_MIN_HEAP_EINVAL;
	if (heap->current_nel == 0)
		return -RUDP_MIN_HEAP_EEMPTY;
	
	size_t i = 0;
	while (bad_invariants(heap, elem, i)) {
		memcpy(getval(heap, i), getval(heap, small_child(heap, i)), heap->elem_size);
		i = small_child(heap, i);
	}
	memcpy(getval(heap, i), elem, heap->elem_size);
	return 0;
}


bool rudp_min_heap_isempty(const struct rudp_min_heap *heap)
{
	if (heap == NULL)
		return true;
	return heap->current_nel == 0;
}
