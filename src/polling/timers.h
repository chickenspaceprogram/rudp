#pragma once
#include <stdint.h>
#include <time.h>
#include <rudp/poll.h>
#include "minheap.h"
#include "time_funcs.h"
#include "id_mgr.h"

struct rudp_timer_internal {
	struct timespec next_alarm;
	struct timespec alarm_interval;
	struct rudp_data data;
	int64_t id;
	uint64_t nrepeats;
};

enum rudp_timerheap_errors {
	RUDP_TIMERHEAP_ISEMPTY = 1,
	RUDP_TIMERHEAP_EINVAL = 2,
	RUDP_TIMERHEAP_EMEM = 3,
	RUDP_TIMERHEAP_EID = 4,
	RUDP_TIMERHEAP_ETMRHEAP = 5,
};

static inline int rudp_timer_internal_cmp(void *t1, void *t2)
{
	struct rudp_timer_internal *timer1 = t1;
	struct rudp_timer_internal *timer2 = t2;
	return timespec_cmp(&(timer1->next_alarm), &(timer2->next_alarm));
	

}

struct rudp_timerheap {
	struct rudp_min_heap heap;
	struct rudp_id_mgr id_mgr;
};

static inline int rudp_timerheap_new(struct rudp_timerheap *timerheap, struct rudp_allocator *alloc)
{
	int res = rudp_id_mgr_new(&(timerheap->id_mgr), alloc);
	if (res < 0) {
		if (res == -RUDP_ID_EINVAL)
			return -RUDP_TIMERHEAP_EINVAL;
		if (res == -RUDP_ID_EMEM)
			return -RUDP_TIMERHEAP_EMEM;
		return -RUDP_TIMERHEAP_EID;
	}
	return rudp_min_heap_new(&(timerheap->heap), alloc, sizeof(struct rudp_timer_internal), rudp_timer_internal_cmp);
}

int64_t rudp_timerheap_add_id(struct rudp_timerheap *timerheap, const struct timespec *cur_time, const struct rudp_timer *tmr);
int rudp_timerheap_add_noid(struct rudp_timerheap *timerheap, const struct timespec *cur_time, const struct rudp_timer *tmr);

int rudp_timerheap_del(struct rudp_timerheap *timerheap, int64_t id);

int rudp_timerheap_time_to_expire(const struct rudp_timerheap *timerheap, const struct timespec *current_time, int64_t *num_ms);

// gets all the expired timers.
// returns the number that are expired (>= 0 if no errors occurred)
int64_t rudp_timerheap_get_expired(struct rudp_timerheap *timerheap, const struct timespec *cur_time, struct rudp_timer_internal *out, size_t nout);


