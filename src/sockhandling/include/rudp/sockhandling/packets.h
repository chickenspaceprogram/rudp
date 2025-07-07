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

struct rudp_packet_header {
	uint32_t magic;
	uint32_t fst_seq_no;
	uint32_t fst_ack_no;
};

static const uint8_t RUDP_SYN = 1;
static const uint8_t RUDP_DATA = 2;
static const uint8_t RUDP_FIN = 3;
static const uint8_t RUDP_RST = 4;

struct rudp_packet_chunk_header {
	uint8_t type;
	uint8_t pktflags;
	uint16_t datalen;
};

struct rudp_packet_chunk_syn {
	uint16_t mss;
	uint16_t max_outstanding_pkts;
	bool ack:1;
};

struct rudp_packet_chunk_data {
	uint16_t datalen;
	uint8_t subindex;
	bool frag:1;
};

