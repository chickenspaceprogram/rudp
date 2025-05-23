#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct rudp_hdr_syn {
	bool ack:1;
	unsigned int version:4;
	bool want_chk:1;
	bool reuse:1;
	uint8_t seq_no;
	uint8_t ack_no;
	uint8_t max_outstanding_segments;
	uint8_t max_retrans;
	uint8_t max_cum_ack;
	uint8_t max_out_of_seq;
	uint8_t max_auto_reset;
	uint16_t max_segment_size;
	uint16_t retrans_timeout_ms;
	uint16_t cum_ack_timeout_ms;
	uint16_t transfer_state_timeout_ms;
	uint32_t conn_id;
};

struct rudp_hdr_ack {
	uint8_t seq_no;
	uint8_t ack_no;
};

struct rudp_hdr_eack {
	uint8_t seq_no;
	uint8_t ack_no;
	uint8_t num_out_of_seq;
	uint8_t *out_of_seq_acks;
};

struct rudp_hdr_rst {
	bool ack:1;
	uint8_t seq_no;
	uint8_t ack_no;
};

struct rudp_hdr_nul {
	uint8_t seq_no;
	uint8_t ack_no;
};

struct rudp_hdr_tcs {
	bool ack:1;
	uint8_t seq_no;
	uint8_t ack_no;
	uint8_t seq_adj_factor;
	uint32_t conn_id;
};

void rudp_hdr_serialize_syn(char *out, struct rudp_hdr_syn packet);
void rudp_hdr_serialize_ack(char *out, struct rudp_hdr_ack packet);
void rudp_hdr_serialize_eack(char *out, struct rudp_hdr_eack packet);
void rudp_hdr_serialize_rst(char *out, struct rudp_hdr_rst packet);
void rudp_hdr_serialize_nul(char *out, struct rudp_hdr_nul packet);
void rudp_hdr_serialize_tcs(char *out, struct rudp_hdr_tcs packet);

static const uint8_t RUDP_SERIAL_SIZE_SYN = 28;
static const uint8_t RUDP_SERIAL_SIZE_ACK = 6;
static const uint8_t RUDP_SERIAL_SIZE_RST = 6;
static const uint8_t RUDP_SERIAL_SIZE_NUL = 6;
static const uint8_t RUDP_SERIAL_SIZE_TCS = 12;

static inline uint8_t rudp_serial_size_eack(struct rudp_hdr_eack packet)
{
	return 6 + packet.num_out_of_seq;
}

uint16_t rudp_checksum(char *hdr, char *body, size_t msg_len);
