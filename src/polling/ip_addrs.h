#pragma once
#include <stdint.h>

struct rudp_ipv4_addr {
	uint8_t addr[4];
	uint8_t port;
};

struct rudp_ipv6_addr {
	uint8_t addr[16];
	uint8_t port;
};
