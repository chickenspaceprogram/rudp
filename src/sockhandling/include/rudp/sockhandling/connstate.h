#pragma once
#include <stdint.h>
#include <rudp/utils/minheap.h>
#include <rudp/sockhandling/packets.h>

struct rudp_packet {
	uint32_t ack_no;
	uint16_t datasize;
	void *data;
};

struct rudp_connection_state {
	uint32_t our_ack_no;
	uint32_t peer_ack_no;
	RUDP_MINHEAP_TYPE(struct rudp_packet) pending_msg_heap;
};
