// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once
#include <stdint.h>
#include <rudp/alloc.h>

enum rudp_id_flags {
	RUDP_ID_PEND_DEL = 0x02,
};

enum rudp_id_errs {
	RUDP_ID_EBADID = 1,
	RUDP_ID_EINVAL = 2,
	RUDP_ID_EMEM = 3,
};

struct rudp_id_mgr {
	uint8_t *idslist;
	size_t idslist_len;
	int64_t min_free_id;
	struct rudp_allocator *alloc;
};

int rudp_id_mgr_new(struct rudp_id_mgr *mgr, struct rudp_allocator *alloc);
void rudp_id_mgr_delete(struct rudp_id_mgr *mgr);

int64_t rudp_id_new(struct rudp_id_mgr *mgr); // Creates a new ID, guaranteed to be distinct from any other ids in the id_mgr.

// this function presumes the arguments are valid
static inline int rudp_id_getflags(const struct rudp_id_mgr *mgr, int64_t rudp_id)
{
	return mgr->idslist[rudp_id] & (~(0x01)); // RUDP_ID_VALID
}

int rudp_id_setflags(struct rudp_id_mgr *mgr, int64_t rudp_id, uint8_t flags);
int rudp_id_del(struct rudp_id_mgr *mgr, int64_t rudp_id);

