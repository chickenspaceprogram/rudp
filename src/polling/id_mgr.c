// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <string.h>
#include "id_mgr.h"
static const uint8_t RUDP_ID_VALID = 0x01;
static const size_t INIT_IDLIST_SIZE = 1024;



int rudp_id_mgr_new(struct rudp_id_mgr *mgr, struct rudp_allocator *alloc)
{
	if (mgr == NULL)
		return -RUDP_ID_EINVAL;
	mgr->idslist = rudp_allocator_alloc(INIT_IDLIST_SIZE * sizeof(uint8_t), alloc);
	if (mgr->idslist == NULL) {
		return -RUDP_ID_EMEM;
	}
	memset(mgr->idslist, 0, INIT_IDLIST_SIZE * sizeof(uint8_t));
	mgr->idslist_len = INIT_IDLIST_SIZE;
	mgr->min_free_id = 0;
	mgr->alloc = alloc;
	return 0;
}


void rudp_id_mgr_delete(struct rudp_id_mgr *mgr)
{
	if (mgr == NULL) {
		return;
	}
	rudp_allocator_free(mgr->idslist, mgr->idslist_len, mgr->alloc);
}

// if this is too slow, i'll implement a min-heap
// for now it's probably fine, i doubt it'll end up iterating more than a few times to find an empty id.
int64_t rudp_id_new(struct rudp_id_mgr *mgr)
{
	if (mgr == NULL) {
		return -RUDP_ID_EINVAL;
	}
	int64_t newid = mgr->min_free_id;
	mgr->idslist[newid] = RUDP_ID_VALID;
	for (size_t i = newid + 1; i < mgr->idslist_len; ++i) {
		if (!(mgr->idslist[i] & RUDP_ID_VALID)) {
			mgr->min_free_id = i;
			break;
		}
	}
	if (newid == mgr->min_free_id) {
		uint8_t *mem = rudp_allocator_reallocarray(mgr->idslist, mgr->idslist_len * 2, mgr->idslist_len, 1, mgr->alloc);
		if (mem == NULL) {
			mgr->idslist[newid] = 0;
			return -RUDP_ID_EMEM;
		}
		memset(mem + mgr->idslist_len, 0, mgr->idslist_len);
		mgr->idslist = mem;
		mgr->min_free_id = mgr->idslist_len;
		mgr->idslist_len *= 2;
	}
	return newid;
}

int rudp_id_setflags(struct rudp_id_mgr *mgr, int64_t rudp_id, uint8_t flags)
{
	if (mgr == NULL) {
		return -RUDP_ID_EINVAL;
	}
	if (rudp_id < 0) {
		return -RUDP_ID_EBADID;
	}
	if ((uint64_t)rudp_id > mgr->idslist_len) {
		return -RUDP_ID_EBADID;
	}
	if (!(mgr->idslist[rudp_id] & RUDP_ID_VALID)) {
		return -RUDP_ID_EBADID;
	}
	mgr->idslist[rudp_id] = flags | RUDP_ID_VALID;
	return 0;
}

int rudp_id_del(struct rudp_id_mgr *mgr, int64_t rudp_id)
{
	if (mgr == NULL) {
		return -RUDP_ID_EINVAL;
	}
	if (rudp_id < 0) {
		return -RUDP_ID_EBADID;
	}
	if ((uint64_t)rudp_id > mgr->idslist_len) {
		return -RUDP_ID_EBADID;
	}
	if (!(mgr->idslist[rudp_id] & RUDP_ID_VALID)) {
		return -RUDP_ID_EBADID;
	}
	mgr->idslist[rudp_id] = 0;
	if (rudp_id < mgr->min_free_id) {
		mgr->min_free_id = rudp_id;
	}
	return 0;
}
