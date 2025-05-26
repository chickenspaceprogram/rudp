#pragma once
#include <stdint.h>
#include <time.h>

#define BILLION 1000000000

static inline struct timespec ms_to_timespec(int64_t ms)
{
	return (struct timespec){
		.tv_sec = (ms / 1000),
		.tv_nsec = (ms % 1000) * 1000000,
	};
}

static inline void timespec_add(struct timespec *out, const struct timespec *in)
{
	out->tv_sec += in->tv_sec;
	if ((BILLION - out->tv_nsec) < in->tv_nsec) {
		++(out->tv_sec);
		out->tv_nsec = in->tv_nsec - (BILLION - out->tv_nsec);
	}
}

// returns num ms difference
static inline int64_t timespec_subtract(const struct timespec *t1, const struct timespec *t2)
{
	return (t1->tv_sec - t2->tv_sec) * 1000 + (t1->tv_nsec - t2->tv_nsec) / 1000000;
}

static inline int timespec_cmp(const void *tm1, const void *tm2)
{
	const struct timespec *time1 = tm1;
	const struct timespec *time2 = tm2;
	if (time1->tv_sec - time2->tv_sec == 0)
		return time1->tv_nsec - time2->tv_nsec;
	else
		return time1->tv_sec - time2->tv_sec;
}

#undef BILLION
