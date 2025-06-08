#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <rudp/utils/alloc.h>

// Any variable starting with RUDP_HASHMAP and ending in _INTERNAL_ is restricted; you may not use these names.

#ifndef RUDP_HASHMAP_INITSIZE
#define RUDP_HASHMAP_INITSIZE 16
#endif

#ifndef RUDP_HASHMAP_MAXLOADFACTOR
#define RUDP_HASHMAP_MAXLOADFACTOR 0.75f
#endif

#define RUDP_HASHMAP_TYPE(KEYTYPE, VALTYPE)\
	struct {\
		struct {\
			KEYTYPE key;\
			VALTYPE val;\
			bool is_filled;\
		} *array;\
		size_t bufsize;\
		size_t nel;\
		size_t (*hashfn)(KEYTYPE *elem);\
		int (*cmp)(KEYTYPE *elem1, KEYTYPE *elem2);\
	}
// god i love the comma operator
#define rudp_hashmap_new(MAPPTR, ALLOCATOR, HASHFN, CMPFN) (\
	(MAPPTR)->array = rudp_allocator_alloc(sizeof(*(MAPPTR)->array) * (RUDP_HASHMAP_INITSIZE), (ALLOCATOR)),\
	memset((MAPPTR)->array, 0, sizeof(*(MAPPTR)->array) * (RUDP_HASHMAP_INITSIZE)),\
	(MAPPTR)->bufsize = 16,\
	(MAPPTR)->nel = 0,\
	(MAPPTR)->hashfn = (HASHFN),\
	(MAPPTR)->cmp = (CMPFN),\
	((MAPPTR)->array == NULL) ? (-1) : 0\
)

#define rudp_hashmap_delete(MAPPTR, ALLOCATOR) do {\
	rudp_allocator_free((MAPPTR)->array, (MAPPTR)->bufsize, (ALLOCATOR));\
} while (0)

#define rudp_hashmap_at(MAPPTR, KEYPTR) ({\
	size_t RUDP_HASHMAP_AT_INDEX_INTERNAL_ = (MAPPTR)->hashfn((KEYPTR)) & ((MAPPTR)->bufsize - 1);\
	typeof((MAPPTR)->array->val) *RUDP_HASHMAP_AT_RETVAL_INTERNAL_ = NULL;\
	while ((MAPPTR)->array[RUDP_HASHMAP_AT_INDEX_INTERNAL_].is_filled) {\
		if ((MAPPTR)->cmp((KEYPTR), &(MAPPTR)->array[RUDP_HASHMAP_AT_INDEX_INTERNAL_].key) == 0) {\
			RUDP_HASHMAP_AT_RETVAL_INTERNAL_ = &(MAPPTR)->array[RUDP_HASHMAP_AT_INDEX_INTERNAL_].val;\
			break;\
		}\
		RUDP_HASHMAP_AT_INDEX_INTERNAL_ = (RUDP_HASHMAP_AT_INDEX_INTERNAL_ + 1) & ((MAPPTR)->bufsize - 1);\
	}\
	RUDP_HASHMAP_AT_RETVAL_INTERNAL_;\
})

#ifdef RUDP_HASHMAP_INSERT_INTERNAL_
#error "RUDP_HASHMAP_INSERT_INTERNAL_ is reserved for the implementation of rudp_hashmap_insert. Please do not define this macro."
#endif

#define RUDP_HASHMAP_INSERT_INTERNAL_(MAPPTR, KEYPTR, VALPTR) do {\
	size_t RUDP_HASHMAP_INSERT_INDEX_INTERNAL_ = (MAPPTR)->hashfn((KEYPTR)) & ((MAPPTR)->bufsize - 1);\
	assert(RUDP_HASHMAP_INSERT_INDEX_INTERNAL_ != 0);\
	while ((MAPPTR)->array[RUDP_HASHMAP_INSERT_INDEX_INTERNAL_].is_filled)\
		RUDP_HASHMAP_INSERT_INDEX_INTERNAL_ = (RUDP_HASHMAP_INSERT_INDEX_INTERNAL_ + 1) & ((MAPPTR)->bufsize - 1);\
	(MAPPTR)->array[RUDP_HASHMAP_INSERT_INDEX_INTERNAL_].is_filled = true;\
	(MAPPTR)->array[RUDP_HASHMAP_INSERT_INDEX_INTERNAL_].key = *(KEYPTR);\
	(MAPPTR)->array[RUDP_HASHMAP_INSERT_INDEX_INTERNAL_].val = *(VALPTR);\
} while (0)

#define rudp_hashmap_reserve(MAPPTR, SPACEAMT, ALLOC) (((MAPPTR)->nel < ((RUDP_HASHMAP_MAXLOADFACTOR) * (MAPPTR)->bufsize)) ? 0 : ({\
	int RUDP_HASHMAP_RESERVE_RETVAL_INTERNAL_ = 0;\
	typeof((MAPPTR)->array) RUDP_HASHMAP_RESERVE_NEWARR_INTERNAL_ = rudp_allocator_alloc((MAPPTR)->bufsize * 2, (ALLOC));\
	typeof((MAPPTR)->array) RUDP_HASHMAP_RESERVE_OLDARR_INTERNAL_ = (MAPPTR)->array;\
	if (RUDP_HASHMAP_RESERVE_NEWARR_INTERNAL_ == NULL) {\
		RUDP_HASHMAP_RESERVE_RETVAL_INTERNAL_ = -1;\
	}\
	else {\
		memset(RUDP_HASHMAP_RESERVE_NEWARR_INTERNAL_, 0, ((MAPPTR)->bufsize * 2));\
		(MAPPTR)->array = RUDP_HASHMAP_RESERVE_NEWARR_INTERNAL_;\
		(MAPPTR)->bufsize *= 2;\
		for (size_t RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_ = 0; RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_ < (MAPPTR)->bufsize / 2; ++RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_) {\
			if (RUDP_HASHMAP_RESERVE_OLDARR_INTERNAL_[RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_].is_filled) {\
				RUDP_HASHMAP_INSERT_INTERNAL_(MAPPTR, &RUDP_HASHMAP_RESERVE_OLDARR_INTERNAL_[RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_].key, &RUDP_HASHMAP_RESERVE_OLDARR_INTERNAL_[RUDP_HASHMAP_RESERVE_INDEX_INTERNAL_].val);\
			}\
		}\
		rudp_allocator_free(RUDP_HASHMAP_RESERVE_OLDARR_INTERNAL_, (MAPPTR)->bufsize / 2, (ALLOC));\
	}\
	RUDP_HASHMAP_RESERVE_RETVAL_INTERNAL_;\
}))


#define rudp_hashmap_insert(MAPPTR, KEYPTR, VALPTR, ALLOC) ((rudp_hashmap_reserve(MAPPTR, (MAPPTR)->bufsize + 1, ALLOC) == 0) ? ({\
	++((MAPPTR)->nel);\
	RUDP_HASHMAP_INSERT_INTERNAL_(MAPPTR, KEYPTR, VALPTR);\
	0;\
}) : -1)

#define RUDP_HASHMAP_BUCKETTYPE(MAPPTR)\
	typeof(*(MAPPTR)->array)

#define rudp_hashmap_bucket_getkey(BUCKETPTR) (&(BUCKETPTR)->key)
#define rudp_hashmap_bucket_getval(BUCKETPTR) (&(BUCKETPTR)->val)

#define RUDP_HASHMAP_ITERTYPE(MAPPTR)\
	struct {\
		typeof(MAPPTR) map;\
		size_t index;\
	}

#define rudp_hashmap_new_iter(ITERPTR, MAPPTR)\
	do {\
		(ITERPTR)->map = MAPPTR;\
		(ITERPTR)->index = 0;\
	} while (0)


// returns something of RUDP_HASHMAP_BUCKETTYPE
#define rudp_hashmap_iter_next(ITERPTR) ({\
	while (!((ITERPTR)->map->array[(ITERPTR)->index].is_filled) && (ITERPTR)->index < (ITERPTR)->map->bufsize)\
		++((ITERPTR)->index);\
	++((ITERPTR)->index);\
	((ITERPTR)->index - 1 < (ITERPTR)->map->bufsize) ? (\
		(ITERPTR)->map->array + (ITERPTR)->index - 1\
	) : NULL;\
})

