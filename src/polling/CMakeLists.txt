# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)

add_library(rudp_polling
	poll.c
	include/rudp/poll.h
)

add_library(rudp_polling_internals
	id_mgr.c
	id_mgr.h
	minheap.c
	minheap.h
	time_funcs.h
	timers.h
	timers.c
)

if (HAVE_EPOLL)
	target_compile_definitions(rudp_polling PRIVATE RUDP_HAVE_EPOLL)
elseif(HAVE_KQUEUE)
	target_compile_definitions(rudp_polling PRIVATE RUDP_HAVE_KQUEUE)
else()
	message(FATAL_ERROR "Failed to find either an epoll or kqueue backend.")
endif()
target_include_directories(rudp_polling PUBLIC include)
target_link_libraries(rudp_polling PUBLIC rudp_mem PRIVATE rudp_polling_internals)
target_link_libraries(rudp_polling_internals PRIVATE rudp_polling)


## tests ##

make_test(tests/test_id_mgr.c PRIVATE rudp_polling rudp_polling_internals)
make_test(tests/test_minheap.c PRIVATE rudp_polling rudp_polling_internals)
