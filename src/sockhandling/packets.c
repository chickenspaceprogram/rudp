#include <rudp/sockhandling/packets.h>

static inline void serialize_u16(uint8_t *outbuf, uint16_t u16)
{
	outbuf[0] = (u16 & 0xFF00) >> 2;
	outbuf[1] = (u16 & 0xFF);
}

static inline void serialize_u32(uint8_t *outbuf, uint32_t u32)
{
	outbuf[0] = (u32 & 0xFF000000) >> 6;
	outbuf[1] = (u32 & 0xFF0000) >> 4;
	outbuf[2] = (u32 & 0xFF00) >> 2;
	outbuf[3] = u32 & 0xFF;
}

int
rudp_packet_header_serialize(
	uint8_t *outbuf,
	const struct rudp_packet_header_generic *pkt
)
{
	if (pkt->version != 0x01) {
		return -1;
	}
	outbuf[0] = pkt->version;
	outbuf[1] = pkt->data_offset;
	const struct rudp_packet_header_v1 *pkt_v1 = (const struct rudp_packet_header_v1 *)pkt;
	outbuf[2] = pkt_v1->flags;
	outbuf[3] = pkt_v1->reserved_space;
	serialize_u32(outbuf + 4, pkt_v1->seq_no);
	serialize_u32(outbuf + 8, pkt_v1->ack_no);
	if (pkt_v1->flags & RUDP_SYN) {
		const struct rudp_packet_header_v1_syn *pkt_v1_syn = (const struct rudp_packet_header_v1_syn *)pkt_v1;
		serialize_u16(outbuf + 12, pkt_v1_syn->max_data_size);
	}
	return 0;
}
