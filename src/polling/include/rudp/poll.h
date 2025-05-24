// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <rudp/alloc.h>


// This polling API is designed to nicely map onto the `epoll` and `kqueue` interfaces.
// illumos does have epoll available, so I haven't taken into account event ports when designing this.

struct rudp_poll;


enum rudp_poll_action {
	RUDP_POLL_ADD = 1,
	RUDP_POLL_MOD = 2,
	RUDP_POLL_DEL = 3,
};

enum rudp_poll_event {
	RUDP_POLL_READ = 0x1,
	RUDP_POLL_WRITE = 0x2,
	RUDP_POLL_ONESHOT = 0x4,
	RUDP_POLL_ET = 0x8,
	RUDP_POLL_ERR = 0x10,
};

struct rudp_data {
	union {
		void *ptr;
		uint64_t u64;
	} data;
	enum {
		RUDP_DATA_PTR,
		RUDP_DATA_U64
	} type;
};


// A struct describing an event.
// `data` is a value that is returned unchanged whenever the event occurs.
//
// `action` signifies the type of action the `rudp_event` is.
// If `action` == RUDP_POLL_ADD, the `rudp_event` will be added to the poll.
// If `action` == RUDP_POLL_MOD, the `rudp_event` present in the poll will be modified.
// If `action` == RUDP_POLL_DEL, the `rudp_event` present in the poll will be deleted.
// In all the above cases, `ident` is interpreted as a file descriptor.
// `events` is interpreted as a bitmask containing various details about which events to listen for.
// Valid flags are listed in rudp_poll_event.
struct rudp_event {
	struct rudp_data data;
	uint16_t action; // one of enum rudp_poll_action
	int fd;
	uint64_t events;
};


struct rudp_timer {
	uint64_t timeout_ms;
	uint64_t nrepeats;
	uint64_t timer_id;
	struct rudp_data data;
};

// Contains details about the event that occurred.
// `data` is the data associated with the event.
// If the event was not a timer, `events` is a bitmask of the events that occurred.
// If the event was a timer, `events` is the remaining number of times the timer will repeat.
struct rudp_event_result {
	struct rudp_data data;
	uint64_t events;
};

// Creates the `rudp_poll` using the given buffer.
// If this function fails, it will return NULL.
// You can find the size of this buffer at runtime with `rudp_poll_size`.
//
// Any memory this function needs will be allocated with the allocator referred to by `alloc`.
// It is guaranteed that if any of these polling functions are called to from the same thread that any memory allocation functions will not be called from different threads
// essentially, don't worry about race conditions if you aren't calling poll functions in a multithreaded context
// they are a factor in a multithreaded ctx though.
struct rudp_poll *rudp_poll_create(struct rudp_allocator *alloc);

int rudp_poll_delete(struct rudp_poll *poll);

// `changes` is nullable if you wish to wait without making changes
//
// The number of events that occurred will be returned.
// If `wait_forever` is true, this will block until an event occurs.
// If not, this will return immediately.
//
// If a timer goes off, 0 will be returned.
// The first element in `out` will have its `data` member set to the data of the timer that went off.
int rudp_poll_wait(
	struct rudp_poll *poll, 
	const struct rudp_event *changes, int nchanges, 
	struct rudp_event_result *out, int nout,
	bool wait_forever
);

int rudp_add_timers(
	struct rudp_poll *poll,
	const struct rudp_timer *timers, int ntimers,
	int64_t *timerids // must be at least ntimers in size, or must be NULL if you do not care about deleting the timers created
);

// Deletes a given timer
int rudp_del_timers(struct rudp_poll *poll, int64_t timerid);

