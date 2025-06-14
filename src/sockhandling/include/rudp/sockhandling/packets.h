#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>


static const uint8_t RUDP_FIN = 0x01; // I've sent everything I will send
static const uint8_t RUDP_SYN = 0x02; // New connection? Here are the rules
static const uint8_t RUDP_RST = 0x04; // Dump all connection state
static const uint8_t RUDP_ACK = 0x08; // Indicates a valid ack_no

struct rudp_packet_header_generic {
	uint8_t version; // packet version
	uint8_t data_offset; // length of packet header, in bytes
};

static const uint8_t RUDP_PACKET_HEADER_GENERIC_SIZE = 2;


struct rudp_packet_header_v1 {
	struct rudp_packet_header_generic hdr;
	uint8_t flags;
	uint8_t reserved_space;
	uint32_t seq_no;
	uint32_t ack_no;
};

static const uint8_t RUDP_PACKET_HEADER_V1_SIZE = 
	RUDP_PACKET_HEADER_GENERIC_SIZE + 2 + 4 + 4;

struct rudp_packet_header_v1_syn {
	struct rudp_packet_header_v1 pkt;
	uint16_t max_data_size;
};


static inline size_t
rudp_packet_header_size(
	const struct rudp_packet_header_generic *pkt
) {
	return pkt->data_offset;
}

int
rudp_packet_header_serialize(
	uint8_t *outbuf,
	const struct rudp_packet_header_generic *pkt
);

static inline void 
rudp_packet_header_v1_syn_new(
	struct rudp_packet_header_v1_syn *syn, 
	uint32_t init_seq_no,
	uint16_t max_datasize
) {
	syn->pkt.hdr.version = 0x01;
	syn->pkt.hdr.data_offset = RUDP_PACKET_HEADER_V1_SIZE + 2;
	syn->pkt.flags = RUDP_SYN;
	syn->pkt.reserved_space = 0;
	syn->pkt.seq_no = init_seq_no; // cringe
	syn->pkt.ack_no = 0;
	syn->max_data_size = max_datasize;
}

static inline void 
rudp_packet_header_v1_synack_new(
	struct rudp_packet_header_v1_syn *syn,
	uint32_t init_seq_no,
	uint32_t init_ack_no,
	uint16_t max_datasize
) {
	syn->pkt.hdr.version = 0x01;
	syn->pkt.hdr.data_offset = RUDP_PACKET_HEADER_V1_SIZE + 2;
	syn->pkt.flags = RUDP_SYN;
	syn->pkt.reserved_space = 0;
	syn->pkt.seq_no = init_seq_no; // cringe
	syn->pkt.ack_no = init_ack_no;
	syn->max_data_size = max_datasize;
}

static inline void 
rudp_packet_header_v1_ack_new(
	struct rudp_packet_header_v1 *ack,
	uint32_t seq_no,
	uint32_t ack_no
) {
	ack->hdr.version = 0x01;
	ack->hdr.data_offset = RUDP_PACKET_HEADER_V1_SIZE;
	ack->flags = RUDP_ACK;
	ack->reserved_space = 0;
	ack->seq_no = seq_no;
	ack->ack_no = ack_no;
}

static inline void 
rudp_packet_header_v1_fin_new(
	struct rudp_packet_header_v1 *fin,
	uint32_t seq_no,
	uint32_t ack_no
) {
	fin->hdr.version = 0x01;
	fin->hdr.data_offset = RUDP_PACKET_HEADER_V1_SIZE;
	fin->flags = RUDP_FIN;
	fin->reserved_space = 0;
	fin->seq_no = seq_no;
	fin->ack_no = ack_no;
}

static inline void 
rudp_packet_header_v1_rst_new(
	struct rudp_packet_header_v1 *rst,
	uint32_t seq_no,
	uint32_t ack_no
) {
	rst->hdr.version = 0x01;
	rst->hdr.data_offset = RUDP_PACKET_HEADER_V1_SIZE;
	rst->flags = RUDP_RST;
	rst->reserved_space = 0;
	rst->seq_no = seq_no;
	rst->ack_no = ack_no;
}

