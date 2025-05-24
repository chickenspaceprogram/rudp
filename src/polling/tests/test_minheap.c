#include "../minheap.h"

#include <stdint.h>
#include <assert.h>

int intcmp(void *n1, void *n2)
{
	int64_t v1 = *(int64_t *)n1;
	int64_t v2 = *(int64_t *)n2;
	return v1 - v2;
}

int main(void)
{
	struct rudp_min_heap *heap = rudp_min_heap_new(NULL, sizeof(int64_t), intcmp);
	int64_t num = 1234;

	assert(rudp_min_heap_peek(heap) == NULL);
	assert(rudp_min_heap_peek(NULL) == NULL);
	assert(rudp_min_heap_pop(heap) == -RUDP_MIN_HEAP_EEMPTY);
	assert(rudp_min_heap_pop(NULL) == -RUDP_MIN_HEAP_EINVAL);
	assert(rudp_min_heap_replace(heap, &num) == -RUDP_MIN_HEAP_EEMPTY);
	assert(rudp_min_heap_isempty(heap));

	rudp_min_heap_insert(heap, &num);
	assert(rudp_min_heap_replace(heap, NULL) == -RUDP_MIN_HEAP_EINVAL);
	assert(rudp_min_heap_replace(NULL, &num) == -RUDP_MIN_HEAP_EINVAL);
	const int64_t *numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(num == *numptr);

	num = 12;
	rudp_min_heap_insert(heap, &num);
	num = 12;
	rudp_min_heap_insert(heap, &num);
	num = 56;
	rudp_min_heap_insert(heap, &num);
	num = 34;
	rudp_min_heap_insert(heap, &num);
	num = -1111;
	rudp_min_heap_insert(heap, &num);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(-1111 == *numptr);
	assert(rudp_min_heap_pop(heap) == 0);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(12 == *numptr);
	assert(rudp_min_heap_pop(heap) == 0);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(12 == *numptr);
	assert(rudp_min_heap_pop(heap) == 0);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(34 == *numptr);
	num = 123;
	assert(rudp_min_heap_replace(heap, &num) == 0);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(56 == *numptr);
	assert(rudp_min_heap_pop(heap) == 0);

	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(123 == *numptr);
	assert(rudp_min_heap_pop(heap) == 0);


	numptr = rudp_min_heap_peek(heap);
	assert(numptr != NULL);
	assert(1234 == *numptr);
	assert(rudp_min_heap_pop(heap) == -RUDP_MIN_HEAP_EEMPTY);

	rudp_min_heap_delete(heap);
}
