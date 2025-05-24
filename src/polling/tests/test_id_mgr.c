#include "../id_mgr.h"
#include <assert.h>

int main(void) {
	struct rudp_id_mgr *id_mgr = rudp_id_mgr_new(NULL);
	assert(id_mgr != NULL);
}
