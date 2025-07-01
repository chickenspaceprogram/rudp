#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

static inline int 
rudp_set_dontfrag(int sock)
{
	int set = 1;
	return setsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &set, sizeof(int));
}

static inline int
rudp_unset_dontfrag(int sock)
{
	int unset = 0;
	return setsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &unset, sizeof(int));
}

static inline int
rudp_get_dontfrag(int sock)
{
	int frag = 0;
	socklen_t len = sizeof(frag);
	if (getsockopt(sock, IPPROTO_IP, IP_DONTFRAG, &frag, &len) == -1) {
		return -1;
	}
	return frag;
}
