#include <rudp/poll.h>

#define RUDP_HAVE_EPOLL // remove this line
#ifdef RUDP_HAVE_EPOLL

#include <sys/epoll.h>

struct rudp_poll {
	int epfd;
	struct rudp_allocator *alloc;
};

struct rudp_poll *rudp_poll_create(struct rudp_allocator *alloc)
{
	struct rudp_poll *mem = rudp_allocator_alloc(sizeof(struct rudp_poll), alloc);
	if (mem == NULL) {
		return NULL;
	}

	mem->alloc = alloc;
	mem->epfd = epoll_create(1234);
	if (mem->epfd == -1) {
		rudp_allocator_free(mem, sizeof(struct rudp_poll), alloc);
		return NULL;
	}
	return mem;
}



#endif

