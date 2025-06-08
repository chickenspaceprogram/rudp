#pragma once
#include <rudp/utils/alloc.h>
#include <stddef.h>
#include <rudp/utils/intmanip.h>

#ifndef RUDP_VECTOR_INIT_SIZE
#define RUDP_VECTOR_INIT_SIZE 8
#endif

#define RUDP_VECTOR_TYPE(ELEMENTS_TYPE)\
	struct {\
		ELEMENTS_TYPE *data;\
		size_t nel;\
		size_t bufsize;\
	}

#define rudp_vector_new(VECPTR, ALLOCATOR_PTR)\
	((VECPTR)->data = rudp_allocator_alloc(sizeof(*(VECPTR)->data) * (RUDP_VECTOR_INIT_SIZE), (ALLOCATOR_PTR)),\
	(VECPTR)->nel = 0,\
	(VECPTR)->bufsize = (RUDP_VECTOR_INIT_SIZE),\
	((VECPTR)->data == NULL) ? -1 : 0)\


#define rudp_vector_delete(VECPTR, ALLOCATOR_PTR)\
	(rudp_allocator_free((VECPTR)->data, (VECPTR)->bufsize, (ALLOCATOR_PTR)))
	

#define rudp_vector_size(VECPTR)	((VECPTR)->nel)
#define rudp_vector_capacity(VECPTR)	((VECPTR)->bufsize)
#define rudp_vector_erase(VECPTR)	(VECPTR)->nel = 0;
#define rudp_vector_empty(VECPTR)	((VECPTR)->nel == 0)


#define rudp_vector_at(VECPTR, INDEX) (((VECPTR)->data) + (INDEX))

#define rudp_vector_push(VECPTR, ELEMPTR, ALLOC)\
_Generic((ELEMPTR), typeof((VECPTR)->data):\
	((rudp_vector_reserve((VECPTR), (VECPTR)->nel + 1, (ALLOC)) == 0) ? (\
	(VECPTR)->data[((VECPTR)->nel)++] = *(ELEMPTR),0\
	) : -1)\
)

#define rudp_vector_pop(VECPTR)\
	(VECPTR)->nel = ((VECPTR)->nel == 0 ? 0 : (VECPTR)->nel - 1);\

// returns 0 if successful, -1 if an error occurs.
// you'll still have to free the vector if an error happens
#define rudp_vector_reserve(VECPTR, NEWSIZE, ALLOC)\
	rudp_vector_reserve_exact((VECPTR), rudp_next_pwr_2((NEWSIZE)), (ALLOC))

#define rudp_vector_reserve_exact(VECPTR, NEWSIZE, ALLOC) ({\
	int retval = 0;\
	if ((NEWSIZE) > (VECPTR)->bufsize) {\
		typeof((VECPTR)->data) newptr = rudp_allocator_reallocarray((VECPTR)->data, (NEWSIZE), (VECPTR)->bufsize, sizeof((VECPTR)->data), (ALLOC));\
		if (newptr != NULL) {\
			(VECPTR)->data = newptr;\
			(VECPTR)->bufsize = (NEWSIZE);\
		}\
		else {\
			retval = -1;\
		}\
	}\
	retval;\
})

