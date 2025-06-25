#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rudp/utils/alloc.h>

struct rudp_addr {
	struct sockaddr_storage addr;
	socklen_t addrlen;
};

struct rudp_packet {
	void *data;
	uint16_t datalen;
	uint32_t seq_no;


	bool syn:1;
	uint16_t max_winsize;
	uint16_t max_segsize;

	bool ack:1;
	uint32_t ack_no;

	bool rst:1;
	bool fin:1;
};

int32_t 
rudp_packet_recv(
	int fd, 
	struct rudp_packet *hdr, 
	struct rudp_addr *addr,
	struct rudp_allocator *alloc
);
int 
rudp_packet_send(
	int fd, 
	const struct rudp_packet *hdr, 
	struct rudp_addr *addr,
	struct rudp_allocator *alloc
);
