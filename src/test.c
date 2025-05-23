#include "headers.h"
#include <stdio.h>

int main(void)
{
	printf("rudp_hdr_syn: %zu\nrudp_hdr_ack: %zu\nrudp_hdr_eack: %zu\n", sizeof(struct rudp_hdr_syn), sizeof(struct rudp_hdr_ack), sizeof(struct rudp_hdr_eack));
}
