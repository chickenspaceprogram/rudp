#pragma once
#include <rudp/utils/minheap.h>
#include <stdint.h>

// need to probably use a min-max heap, this would allow popping ids 1 less than the next_free_id from the freelist
// this should work though in any case

struct rudp_uid_gen {
	RUDP_MINHEAP_TYPE(int32_t) freelist;
	int32_t next_free_id;
	struct rudp_allocator *alloc;
};

static inline int rudp_uid_gen_new(struct rudp_uid_gen *gen, struct rudp_allocator *alloc)
{
	if (rudp_minheap_new(gen->freelist, alloc) != 0)
		return -1;
	gen->next_free_id = 0;
	return 0;
}

static inline int rudp_uid_cmp(int32_t id1, int32_t id2)
{
	return id1 - id2;
}

static inline int32_t rudp_uid_gen_register(struct rudp_uid_gen *gen)
{
	if (rudp_minheap_empty(gen->freelist)) {
		return (gen->next_free_id)++;
	}
	int32_t result = rudp_minheap_top(gen->freelist);
	rudp_minheap_pop(gen->freelist, rudp_uid_cmp);
	return result;
}
static inline int rudp_uid_gen_deregister(struct rudp_uid_gen *gen, int32_t uid)
{
	if (uid == gen->next_free_id - 1) {
		--(gen->next_free_id);
		return 0;
	}
	return rudp_minheap_push(gen->freelist, uid, rudp_uid_cmp, gen->alloc);
}

