cmake_minimum_required(VERSION 3.12)

add_library(rudp_polling
	id_mgr.c
	id_mgr.h
	minheap.c
	minheap.h
	poll.c
	include/rudp/poll.h
)

target_include_directories(rudp_polling PUBLIC include)
target_link_libraries(rudp_polling PUBLIC rudp_mem)
