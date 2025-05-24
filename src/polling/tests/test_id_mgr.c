#include "../id_mgr.h"
#include <assert.h>


// these tests suck tbh
int main(void)
{
	struct rudp_id_mgr *id_mgr = rudp_id_mgr_new(NULL);
	assert(id_mgr != NULL);

	int64_t ids[8];
	for (size_t i = 0; i < 8; ++i) {
		ids[i] = rudp_id_new(id_mgr);
		assert(ids[i] == i);
		assert(rudp_id_setflags(id_mgr, ids[i], RUDP_ID_PEND_DEL) == 0);
		assert(rudp_id_getflags(id_mgr, ids[i]) == RUDP_ID_PEND_DEL);
	}

	assert(rudp_id_del(id_mgr, ids[3]) == 0);
	assert(rudp_id_new(id_mgr) == 3);

	rudp_id_mgr_delete(id_mgr);
}
