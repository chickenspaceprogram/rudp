#pragma once
#include <stdint.h>
#include "minheap.h"
#include <rudp/poll.h>

struct rudp_timer_internal {
	uint64_t timeout;
	uint64_t nrepeats;
	struct rudp_data data;
	int64_t id;
};

struct rudp_timerheap {
	struct rudp_min_heap *heap;
};

struct rudp_min_heap *rudp_make_timerheap(void);
int rudp_timerheap_add(struct rudp_min_heap *timerheap, const struct rudp_timer *tmr);

int rudp_timerheap_del

