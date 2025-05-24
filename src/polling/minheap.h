#pragma once
#include <stdbool.h>
#include <rudp/alloc.h>

struct rudp_min_heap;

enum rudp_min_heap_errors {
	RUDP_MIN_HEAP_EEMPTY = 1,
	RUDP_MIN_HEAP_EINVAL = 2,
	RUDP_MIN_HEAP_EMEM = 3,
};

struct rudp_min_heap *rudp_min_heap_new(struct rudp_allocator *alloc, size_t element_size, int (*cmp)(void *, void *));
void rudp_min_heap_delete(struct rudp_min_heap *heap);

void *rudp_min_heap_peek(const struct rudp_min_heap *heap);

int rudp_min_heap_insert(struct rudp_min_heap *heap, void *item);

int rudp_min_heap_pop(struct rudp_min_heap *heap);

// Pops the element on top of the heap, then pushes `elem` onto the heap.
int rudp_min_heap_replace(struct rudp_min_heap *heap, void *elem);

bool rudp_min_heap_isempty(struct rudp_min_heap *heap);

