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

static inline void add_to_timespec(struct timespec *out, const struct timespec *in)
{
	out->tv_sec += in->tv_sec;
	if ((BILLION - out->tv_nsec) < in->tv_nsec) {
		++(out->tv_sec);
		out->tv_nsec = in->tv_nsec - (BILLION - out->tv_nsec);
	}
}

static inline int timespec_cmp(void *tm1, void *tm2)
{
	struct timespec *time1 = tm1;
	struct timespec *time2 = tm2;
	if (time1->tv_sec - time2->tv_sec == 0)
		return time1->tv_nsec - time2->tv_nsec;
	else
		return time1->tv_sec - time2->tv_sec;
}

#undef BILLION
