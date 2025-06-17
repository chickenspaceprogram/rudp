#pragma once

#include <rudp/utils/alloc.h>
#include <rudp/utils/vector.h>
#include <stddef.h>

#ifndef RUDP_DEQUE_INITSIZE
#define RUDP_DEQUE_INITSIZE 16
#endif

#define RUDP_DEQUE_TYPE(TYPENAME)\
struct {\
	TYPENAME *array;\
	size_t arrsize;\
	size_t firstel;\
	size_t nel;\
}

#define rudp_deque_new(DEQUE, ALLOC)\
	((DEQUE).array = rudp_allocator_allocarray(RUDP_DEQUE_INITSIZE, sizeof(*((DEQUE).array)), (ALLOC)),\
	(((DEQUE).array == NULL) ? -1 : (\
	(DEQUE).arrsize = 16,\
	(DEQUE).firstel = 0,\
	(DEQUE).nel = 0,\
	0)))

#define rudp_deque_at(DEQUE, INDEX)\
	((DEQUE).array[((DEQUE).firstel + (INDEX)) & ((DEQUE).arrsize - 1)])
#define rudp_deque_front(DEQUE) rudp_deque_at((DEQUE), 0)
#define rudp_deque_back(DEQUE) rudp_deque_at((DEQUE), (DEQUE).nel)
#define rudp_deque_size(DEQUE) ((DEQUE).nel)
#define rudp_deque_capacity(DEQUE) ((DEQUE).arrsize)

#define rudp_deque_reserve(DEQUE, AMOUNT, ALLOC) ({\
	int RUDP_DEQUE_RESERVE_RETVAL_INTERNAL_ = 0;\
	if ((AMOUNT) > (DEQUE).arrsize) {\
		typeof((DEQUE).array) RUDP_DEQUE_RESERVE_NEWARR_INTERNAL_ = rudp_allocator_reallocarray((DEQUE).array, rudp_next_pwr_2(AMOUNT), (DEQUE).arrsize, sizeof(*((DEQUE).array)), (ALLOC));\
		if (RUDP_DEQUE_RESERVE_NEWARR_INTERNAL_ == NULL) {\
			RUDP_DEQUE_RESERVE_RETVAL_INTERNAL = NULL;\
		}\
		else {\
		}\
	}\
	RUDP_DEQUE_RESERVE_RETVAL_INTERNAL_;\
})

#define rudp_deque_push_back
#define rudp_deque_pop_back
#define rudp_deque_push_front
#define rudp_deque_pop_front

