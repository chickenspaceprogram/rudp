// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include <stdbool.h>
#include <rudp/utils/alloc.h>

struct rudp_min_heap {
	char *elements;
	size_t elem_size;
	size_t max_nel;
	size_t current_nel;
	int (*cmp)(void *, void *);
	struct rudp_allocator *alloc;
};

enum rudp_min_heap_errors {
	RUDP_MIN_HEAP_EEMPTY = 1,
	RUDP_MIN_HEAP_EINVAL = 2,
	RUDP_MIN_HEAP_EMEM = 3,
};

int rudp_min_heap_new(struct rudp_min_heap *heap, struct rudp_allocator *alloc, size_t element_size, int (*cmp)(void *, void *));
void rudp_min_heap_delete(struct rudp_min_heap *heap);

void *rudp_min_heap_peek(const struct rudp_min_heap *heap);

int rudp_min_heap_insert(struct rudp_min_heap *heap, void *item);

int rudp_min_heap_pop(struct rudp_min_heap *heap);

// Pops the element on top of the heap, then pushes `elem` onto the heap.
int rudp_min_heap_replace(struct rudp_min_heap *heap, void *elem);

bool rudp_min_heap_isempty(const struct rudp_min_heap *heap);

