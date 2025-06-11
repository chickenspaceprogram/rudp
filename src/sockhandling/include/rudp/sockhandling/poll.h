#pragma once
#include <sys/socket.h>
#include <stdint.h>

enum rudp_poll_requesttype {
	RUDP_POLL_TIMER,
	RUDP_POLL_SOCKET,
};

struct rudp_poll_timer {
	enum rudp_poll_requesttype rtype; // set to RUDP_POLL_TIMER
	int timeout;
	struct sockaddr_storage addr;
};

struct rudp_poll_socket {
	enum rudp_poll_requesttype rtype;
	struct sockaddr_storage addrmask;
	uint16_t max_msgsize;
};

