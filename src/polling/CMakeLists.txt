# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
