#pragma once
#include <stdint.h>
#include <sys/socket.h>
#include <rudp/utils/hashmap.h>
#include <rudp/utils/deque.h>
#include <rudp/sockhandling/packets.h>

struct rudp_syn_info {
	uint32_t seq_no;
	uint16_t max_sgmt_size;
	uint16_t recv_win_size;
};

struct rudp_conn_settings {
	uint32_t time_wait_len;
	uint16_t resend_timeout; // resend msgs in window this often
	uint16_t ack_timeout; // send an ack packet this often
	uint16_t failure_resend_timeout; // if we get EWOULDBLOCK, retry after this time
	uint8_t max_retrans; // this many retrans before considered broken
	uint8_t current_retrans;
};

enum rudp_states {
	RUDP_CLOSED,
	RUDP_SYN_SENT,
	RUDP_LISTEN,
	RUDP_SYN_RECV,
	RUDP_ESTABLISHED,
	RUDP_FIN_WAIT_1,
	RUDP_FIN_WAIT_2,
	RUDP_CLOSING,
	RUDP_TIME_WAIT,
	RUDP_CLOSE_WAIT,
	RUDP_LAST_ACK,
};

struct rudp_syn_sent {
	struct rudp_syn_info our_syn;
	struct rudp_conn_settings settings;
	struct sockaddr_storage peer_addr;
};

struct rudp_listen {
	uint16_t max_sgmt_size;
	uint16_t recv_win_size;
	int sockfd;
};

struct rudp_syn_recv {
	struct rudp_syn_info our_syn;
	struct rudp_syn_info peer_syn;
	struct rudp_conn_settings settings;
	struct sockaddr_storage peer_addr;
};

struct rudp_established {
	struct rudp_syn_info our_data;
	struct rudp_syn_info peer_data;
	struct rudp_conn_settings settings;
	struct sockaddr_storage peer_addr;
	RUDP_HASHMAP_TYPE(uint32_t, struct rudp_packet) pending_recv_msgs;
	RUDP_DEQUE_TYPE(struct rudp_packet) ready_recv_msgs;
	RUDP_DEQUE_TYPE(struct rudp_packet) pending_send_msgs;
};

