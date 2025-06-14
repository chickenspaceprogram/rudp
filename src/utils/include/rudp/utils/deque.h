#pragma once

#include <rudp/utils/alloc.h>
#include <stddef.h>

#ifndef RUDP_DEQUE_NODE_SIZE
#define RUDP_DEQUE_NODE_SIZE 32
#endif

#define RUDP_DEQUE_NODE_TYPE_INTERNAL_(ELTYPE)\
struct {\
	RUDP_DEQUE_NODE_TYPE_INTERNAL_(ELTYPE) *next;\
	RUDP_DEQUE_NODE_TYPE_INTERNAL_(ELTYPE) *last;\
	size_t firstel;\
	size_t lastel;\
	ELTYPE data[RUDP_DEQUE_NODE_SIZE];\
}

#define RUDP_DEQUE_TYPE(ELTYPE)\
struct {\
	RUDP_DEQUE_NODE_TYPE_INTERNAL_(ELTYPE) *first;\
	RUDP_DEQUE_NODE_TYPE_INTERNAL_(ELTYPE) *last;\
}

#define rudp_deque_new(DEQUE) do {\
	(DEQUE).first = NULL;\
	(DEQUE).last = NULL;\
} while (0)

#define rudp_deque_front(DEQUE) ((DEQUE).first->data[(DEQUE).first->firstel])

#define rudp_deque_back(DEQUE) ((DEQUE).last->data[(DEQUE).last->lastel])

#define rudp_deque_push_back(DEQUE, ELEM, ALLOCPTR) ({\
	int RUDP_DEQUE_PUSH_BACK_RETVAL_INTERNAL_ = 0;\
	if ((DEQUE).first == NULL) {\
		(DEQUE).first = rudp_allocator_alloc(sizeof(*((DEQUE).first)), (ALLOCPTR));\
		if ((DEQUE).first != NULL) {\
			(DEQUE).last = (DEQUE).first;\
			(DEQUE).last.next = NULL;\
			(DEQUE).last.last = NULL;\
			(DEQUE).last.firstel = RUDP_DEQUE_NODE_SIZE / 2;\
			(DEQUE).last.lastel = RUDP_DEQUE_NODE_SIZE / 2;\
			(DEQUE).last.data[RUDP_DEQUE_NODE_SIZE / 2] = ELEM;\
		}\
		else {\
			RUDP_DEQUE_PUSH_BACK_RETVAL_INTERNAL_ = -1;\
		}\
	}\
	else {\
		if ((DEQUE).last.lastel == RUDP_DEQUE_NODE_SIZE - 1) {\
			(DEQUE).last.next = rudp_allocator_alloc(sizeof(*((DEQUE).first)), (ALLOCPTR));\
			if ((DEQUE).last.next != NULL) {\
				(DEQUE).last.next.last = (DEQUE).last;\
				(DEQUE).last = (DEQUE).last.next;\
				(DEQUE).last.next = NULL;\
				(DEQUE).last.firstel = 0;\
				(DEQUE).last.lastel = 0;\
				(DEQUE).last.data[0] = ELEM;\
			}\
			else {\
				RUDP_DEQUE_PUSH_BACK_RETVAL_INTERNAL_ = -1;\
			}\
		}\
		else {\
			(DEQUE).last.data[++((DEQUE).last.lastel)] = ELEM;\
		}\
	}\
	RUDP_DEQUE_PUSH_BACK_RETVAL_INTERNAL_;\
})

#define rudp_deque_pop_back(DEQUE, ALLOCPTR) do {\
	if ((DEQUE).last != NULL) {\
		if ((DEQUE).last.firstel == (DEQUE).last.lastel) {\
			if ((DEQUE).last.last == NULL) {\
				rudp_allocator_free((DEQUE).last, sizeof(*((DEQUE).last)), (ALLOCPTR));\
				(DEQUE).first = NULL;\
				(DEQUE).last = NULL;\
			}\
			else {\
				(DEQUE).last = (DEQUE).last.last;\
				rudp_allocator_free((DEQUE).last.next, sizeof(*((DEQUE).last.next)), (ALLOCPTR));\
				(DEQUE).last.next = NULL;\
			}\
		}\
		else {\
			--((DEQUE).last.lastel);\
		}\
	}\
} while (0)

#define rudp_deque_push_front(DEQUE, ELEM, ALLOCPTR) ({\
	int RUDP_DEQUE_PUSH_FRONT_RETVAL_INTERNAL_ = 0;\
	if ((DEQUE).first == NULL) {\
		(DEQUE).first = rudp_allocator_alloc(sizeof(*((DEQUE).first)), (ALLOCPTR));\
		if ((DEQUE).first != NULL) {\
			(DEQUE).last = (DEQUE).first;\
			(DEQUE).last.next = NULL;\
			(DEQUE).last.last = NULL;\
			(DEQUE).last.firstel = RUDP_DEQUE_NODE_SIZE / 2;\
			(DEQUE).last.lastel = RUDP_DEQUE_NODE_SIZE / 2;\
			(DEQUE).last.data[RUDP_DEQUE_NODE_SIZE / 2] = ELEM;\
		}\
		else {\
			RUDP_DEQUE_PUSH_FRONT_RETVAL_INTERNAL_ = -1;\
		}\
	}\
	else {\
		if ((DEQUE).first.firstel == 0) {\
			(DEQUE).first.last = rudp_allocator_alloc(sizeof(*((DEQUE).first)), (ALLOCPTR));\
			if ((DEQUE).first.last != NULL) {\
				(DEQUE).first.last.next = (DEQUE).first;\
				(DEQUE).first = (DEQUE).first.last;\
				(DEQUE).first.last = NULL;\
				(DEQUE).first.firstel = RUDP_DEQUE_NODE_SIZE - 1;\
				(DEQUE).first.lastel = RUDP_DEQUE_NODE_SIZE - 1;\
				(DEQUE).first.data[RUDP_DEQUE_NODE_SIZE - 1] = ELEM;\
			}\
			else {\
				RUDP_DEQUE_PUSH_FRONT_RETVAL_INTERNAL_ = -1;\
			}\
		}\
		else {\
			(DEQUE).first.data[--((DEQUE).first.firstel)] = ELEM;\
		}\
	}\
	RUDP_DEQUE_PUSH_FRONT_RETVAL_INTERNAL_;\
})

#define rudp_deque_pop_front(DEQUE, ALLOCPTR) do {\
	if ((DEQUE).first != NULL) {\
		if ((DEQUE).first.firstel == (DEQUE).first.lastel) {\
			if ((DEQUE).first.next == NULL) {\
				rudp_allocator_free((DEQUE).first, sizeof(*((DEQUE).first)), (ALLOCPTR));\
				(DEQUE).first = NULL;\
				(DEQUE).last = NULL;\
			}\
			else {\
				(DEQUE).first = (DEQUE).first.next;\
				rudp_allocator_free((DEQUE).first.last, sizeof(*((DEQUE).first.last)), (ALLOCPTR));\
				(DEQUE).first.last = NULL;\
			}\
		}\
		else {\
			++((DEQUE).first.firstel);\
		}\
	}\
} while (0)

