#pragma once
#include <stdint.h>
#include <stddef.h>

struct rudp_configurables {
	// non-negotiable
	uint16_t max_segment_size;
	uint8_t max_outstanding_segments;

	// negotiable
	bool data_checksum:1;
	uint16_t retrans_timeout;
	uint16_t cum_ack_timeout;
	uint16_t nul_timeout;
	uint16_t transfer_state_timeout;
	uint8_t max_retrans;
	uint8_t max_cum_ack;
	uint8_t max_out_of_seq;
	uint8_t max_auto_reset;
	size_t input_queue_size;
};

static const struct rudp_configurables RUDP_DEFAULTS = {
	.max_segment_size = 0xFFFF, // picked because it seems legit
	.max_outstanding_segments = 3, // also picked because it seems right

	// following are from the IETF draft
	.data_checksum = true,
	.retrans_timeout = 600,
	.cum_ack_timeout = 300,
	.nul_timeout = 2000,
	.transfer_state_timeout = 1000,
	.max_retrans = 2,
	.max_cum_ack = 3,
	.max_out_of_seq = 3,
	.max_auto_reset = 3,
	.input_queue_size = 32,
};

struct rudp_connection_state {
	unsigned int rudp_version:4;
	uint32_t our_connid;
	uint32_t peer_connid;

};
