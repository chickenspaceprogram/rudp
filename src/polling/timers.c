#include "timers.h"

int64_t rudp_timerheap_add_id(struct rudp_timerheap *timerheap, const struct timespec *cur_time, const struct rudp_timer *tmr)
{
	if (timerheap == NULL || cur_time == NULL || tmr == NULL) {
		return -RUDP_TIMERHEAP_EINVAL;
	}
	struct timespec alarm_interval = ms_to_timespec(tmr->timeout_ms);
	struct rudp_timer_internal new_tmr = {
		.next_alarm = alarm_interval,
		.alarm_interval = alarm_interval,
		.data = tmr->data,
		.id = rudp_id_new(&(timerheap->id_mgr)),
		.nrepeats = tmr->nrepeats,
	};
	if (new_tmr.id < 0)
		return -RUDP_TIMERHEAP_EID;
	timespec_add(&(new_tmr.next_alarm), cur_time);
	return rudp_min_heap_insert(&(timerheap->heap), &(new_tmr));
}

int rudp_timerheap_add_noid(struct rudp_timerheap *timerheap, const struct timespec *cur_time, const struct rudp_timer *tmr)
{
	if (timerheap == NULL || cur_time == NULL || tmr == NULL) {
		return -RUDP_TIMERHEAP_EINVAL;
	}
	struct timespec alarm_interval = ms_to_timespec(tmr->timeout_ms);
	struct rudp_timer_internal new_tmr = {
		.next_alarm = alarm_interval,
		.alarm_interval = alarm_interval,
		.data = tmr->data,
		.id = -1,
		.nrepeats = tmr->nrepeats,
	};
	timespec_add(&(new_tmr.next_alarm), cur_time);
	return rudp_min_heap_insert(&(timerheap->heap), &(new_tmr));
}

int rudp_timerheap_del(struct rudp_timerheap *timerheap, int64_t id)
{
	if (timerheap == NULL || id < 0)
		return -RUDP_TIMERHEAP_EINVAL;
	if (rudp_min_heap_isempty(&(timerheap->heap)))
		return -RUDP_TIMERHEAP_ISEMPTY;

	const struct rudp_timer_internal *top = rudp_min_heap_peek(&(timerheap->heap));
	if (top == NULL)
		return -RUDP_TIMERHEAP_EID;
	if (top->id == id) {
		rudp_min_heap_pop(&(timerheap->heap));
		rudp_id_del(&(timerheap->id_mgr), id);
		return 0;
	}

	int result = rudp_id_setflags(&(timerheap->id_mgr), id, RUDP_ID_PEND_DEL);
	if (result == RUDP_ID_EMEM)
		return -RUDP_TIMERHEAP_EMEM;
	if (result == RUDP_ID_EINVAL)
		return -RUDP_TIMERHEAP_EINVAL;
	if (result == RUDP_ID_EBADID)
		return -RUDP_TIMERHEAP_EID;
	return 0;
}

int rudp_timerheap_time_to_expire(const struct rudp_timerheap *timerheap, const struct timespec *current_time, int64_t *num_ms)
{
	if (timerheap == NULL || current_time == NULL)
		return -RUDP_TIMERHEAP_EINVAL;
	if (rudp_min_heap_isempty(&(timerheap->heap)))
		return -RUDP_TIMERHEAP_ISEMPTY;
	const struct rudp_timer_internal *top = rudp_min_heap_peek(&(timerheap->heap));
	if (top == NULL)
		return -RUDP_TIMERHEAP_EID;
	*num_ms = timespec_subtract(&(top->next_alarm), current_time);
	return 0;
}

int64_t rudp_timerheap_get_expired(struct rudp_timerheap *timerheap, const struct timespec *cur_time, struct rudp_timer_internal *out, size_t nout)
{
	size_t i = 0;
	if (nout == 0 || timerheap == NULL || cur_time == NULL || out == NULL)
		return -RUDP_TIMERHEAP_EINVAL;
	while (true) {
		if (i == nout)
			return i;
		struct rudp_timer_internal *top = rudp_min_heap_peek(&(timerheap->heap));
		if (top == NULL || timespec_cmp(cur_time, &(top->next_alarm)) < 0)
			return i;

		if (top->id > 0 && rudp_id_getflags(&(timerheap->id_mgr), top->id) & RUDP_ID_PEND_DEL) {
			rudp_min_heap_pop(&(timerheap->heap));
			continue;
		}

		--(top->nrepeats);
		if (top->nrepeats > 0) {
			out[i++] = *top;
			struct rudp_timer_internal newtop = *top;
			timespec_add(&(newtop.next_alarm), &(newtop.alarm_interval));
			int e = rudp_min_heap_pop(&(timerheap->heap));
			if (e != 0)
				return -RUDP_TIMERHEAP_ETMRHEAP;
			e = rudp_min_heap_insert(&(timerheap->heap), &newtop);
			if (e != 0)
				return -RUDP_TIMERHEAP_ETMRHEAP;
		}
		else {
			out[i++] = *top;
			rudp_min_heap_pop(&(timerheap->heap));
		}
	}
}
