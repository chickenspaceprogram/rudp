#include <rudp/utils/serialize.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
	uint8_t buf[8];
	uint8_t n1 = 0x12;
	uint16_t n2 = 0x3456;
	uint32_t n3 = 0x78901234;
	uint64_t n4 = 0x5678901234567890;

	memset(buf, 0, 8);
	rudp_serialize_int(buf, n1);
	assert(*buf == 0x12);
	memset(buf, 0, 8);

	rudp_serialize_int(buf, n2);
	assert(buf[0] == 0x34);
	assert(buf[1] == 0x56);
	memset(buf, 0, 8);

	rudp_serialize_int(buf, n3);
	assert(buf[0] == 0x78);
	assert(buf[1] == 0x90);
	assert(buf[2] == 0x12);
	assert(buf[3] == 0x34);
	memset(buf, 0, 8);

	rudp_serialize_int(buf, n4);
	assert(buf[0] == 0x56);
	assert(buf[1] == 0x78);
	assert(buf[2] == 0x90);
	assert(buf[3] == 0x12);
	assert(buf[4] == 0x34);
	assert(buf[5] == 0x56);
	assert(buf[6] == 0x78);
	assert(buf[7] == 0x90);
	memset(buf, 0, 8);
}
