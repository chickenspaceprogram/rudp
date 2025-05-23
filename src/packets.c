#include "packets.h"

enum rudp_msg_types {
	RUDP_SYN = 0x80,
	RUDP_ACK = 0x40,
	RUDP_EACK = 0x20,
	RUDP_RST = 0x10,
	RUDP_NUL = 0x08,
	RUDP_CHK = 0x04,
	RUDP_TCS = 0x02,
};

enum rudp_syn_option_flags {
	RUDP_SYN_WANT_CHK = 0x40,
	RUDP_SYN_REUSE = 0x20,
};

static inline uint16_t to_1s_comp(uint8_t byte)
{
	if (byte & 0x80) { // highest bit set
		return -(int8_t)(~byte);
	}
	else {
		return byte;
	}
}

void rudp_hdr_serialize_syn(char *out, struct rudp_hdr_syn packet)
{
	*out = 0;
	if (packet.ack) {
		*out = RUDP_ACK;
	}
	*out++ |= RUDP_SYN;
	*out++ = 28;
	*out++ = packet.seq_no;
	*out++ = packet.ack_no;
	*out++ = packet.version << 4;
	*out++ = packet.max_outstanding_segments;
	*out = 0x80;
	if (packet.want_chk) {
		*out |= RUDP_SYN_WANT_CHK;
	}
	if (packet.reuse) {
		*out |= RUDP_SYN_REUSE;
	}
	++out;

}

void rudp_hdr_serialize_ack(char *out, struct rudp_hdr_ack packet);
void rudp_hdr_serialize_eack(char *out, struct rudp_hdr_eack packet);
void rudp_hdr_serialize_rst(char *out, struct rudp_hdr_rst packet);
void rudp_hdr_serialize_nul(char *out, struct rudp_hdr_nul packet);
void rudp_hdr_serialize_tcs(char *out, struct rudp_hdr_tcs packet);

uint16_t rudp_checksum(char *hdr, char *body, size_t body_len)
{
	uint16_t accum = 0;
	uint8_t hdr_len = hdr[1];
	for (size_t i = 0; i < hdr_len; ++i) {
		accum += to_1s_comp(hdr[i]);
	}

	if (body != NULL) {
		for (size_t i = 0; i < body_len; ++i) {
			accum += to_1s_comp(body[i]);
		}
	}
	
	return accum;
}
