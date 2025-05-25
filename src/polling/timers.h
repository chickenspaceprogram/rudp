#pragma once
#include <stdint.h>
#include "minheap.h"
#include <rudp/poll.h>
#include <time.h>

struct rudp_timer_internal {
	int64_t timeout;
	struct rudp_data data;
	int64_t id;
	uint64_t nrepeats;
};

struct rudp_timerheap {
	struct rudp_min_heap *heap;
};

int rudp_timerheap_new(struct rudp_timerheap *timerheap);

int rudp_timerheap_add(struct rudp_timerheap *timerheap, const struct timespec *cur_time, const struct rudp_timer_internal *tmr);
int rudp_timerheap_del(struct rudp_timerheap *timerheap, int64_t id);

int64_t rudp_timerheap_get_wait(struct rudp_timerheap *timerheap, const struct timespec *cur_time);
int64_t rudp_timerheap_get_expired(struct rudp_timerheap *timerheap, const struct timespec *cur_time, struct rudp_timer_internal *out, size_t nout);


