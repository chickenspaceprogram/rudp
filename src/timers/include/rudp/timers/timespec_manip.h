#pragma once
#include <time.h>
#include <stdint.h>

static inline struct timespec rudp_timespec_add(struct timespec tm1, struct timespec tm2)
{
	static const uint32_t BILLION = 1000000000;
	struct timespec result = {.tv_sec = tm1.tv_sec + tm2.tv_sec};
	if ((BILLION - tm1.tv_nsec) <= tm2.tv_nsec) {
		++(result.tv_sec);
		result.tv_nsec = tm2.tv_nsec - (BILLION - tm1.tv_nsec);
	}
	else {
		result.tv_nsec = tm1.tv_nsec + tm2.tv_nsec;
	}
	return result;
}

static inline struct timespec rudp_timespec_add_sec(struct timespec tm1, time_t sec)
{
	return (struct timespec){.tv_sec = tm1.tv_sec + sec, .tv_nsec = tm1.tv_nsec};
}

static inline struct timespec rudp_timespec_add_msec(struct timespec tm1, unsigned int ms)
{
	static const uint32_t BILLION = 1000000000;
	static const uint32_t MILLION = 1000000;
	unsigned int mod_ms = ms % 1000;
	struct timespec result = {.tv_sec = tm1.tv_sec + ms / 1000};
	if (BILLION - MILLION * mod_ms <= tm1.tv_nsec) {
		result.tv_nsec = tm1.tv_nsec - (BILLION - MILLION * mod_ms);
		++(result.tv_sec);
	}
	else {
		result.tv_nsec = MILLION * mod_ms + tm1.tv_nsec;
	}
	return result;
}

static inline int rudp_timespec_cmp(struct timespec tm1, struct timespec tm2)
{
	return (tm1.tv_sec - tm2.tv_sec == 0) ? tm1.tv_nsec - tm2.tv_nsec : tm1.tv_sec - tm2.tv_sec;
}
