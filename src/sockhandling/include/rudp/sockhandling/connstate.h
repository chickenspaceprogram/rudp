#pragma once
#include <stdint.h>
#include <rudp/utils/hashmap.h>
#include <rudp/utils/deque.h>
#include <rudp/sockhandling/packets.h>

struct rudp_packet {
	void *data;
	uint16_t datasize;
};

struct rudp_connection_state {
	uint32_t our_ack_no;
	uint32_t peer_ack_no;
	uint64_t peer_timestamp;
	RUDP_HASHMAP_TYPE(uint32_t, struct rudp_packet) pending_recv_msgs;
	RUDP_DEQUE_TYPE(struct rudp_packet) ready_recv_msgs;
	RUDP_DEQUE_TYPE(struct rudp_packet) pending_send_msgs;

};
