#include <rudp/sockhandling/packets.h>

static const uint8_t RUDP_SYN = 0x80;
static const uint8_t RUDP_ACK = 0x40;
static const uint8_t RUDP_FIN = 0x20;
static const uint8_t RUDP_RST = 0x10;


struct hdrinfo {
	uint8_t flags;
	unsigned int version:4;
	unsigned int hdrlen:4;
	uint16_t datalen;
	uint32_t seq_no;
};

static void write_u16(uint8_t *outbuf, uint16_t n)
{
	*(outbuf++) = (n & 0xFF00) >> 8;
	*(outbuf++) = n & 0xFF;
}
static void write_u32(uint8_t *outbuf, uint32_t n)
{
	*(outbuf++) = (n & 0xFF000000) >> 24;
	*(outbuf++) = (n & 0xFF0000) >> 16;
	*(outbuf++) = (n & 0xFF00) >> 8;
	*(outbuf++) = n & 0xFF;
}

static void makeheader(uint8_t *outbuf, const struct hdrinfo *info)
{
	*(outbuf++) = info->flags;
	*(outbuf++) = (info->version << 4) | info->hdrlen;
	write_u16(outbuf, info->datalen);
	outbuf += 2;
	write_u32(outbuf, info->seq_no);
}

static void makesyn(uint8_t *outbuf, uint16_t max_winsize, uint16_t max_datasize)
{
	write_u16(outbuf, max_winsize);
	write_u16(outbuf + 2, max_datasize);
}

static void makeack(uint8_t *outbuf, uint32_t ack_no)
{
	write_u32(outbuf, ack_no);
}

int main(void)
{
}
