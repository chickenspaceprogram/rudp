# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)

add_library(rudp_sockhandling
	include/rudp/sockhandling/timespec_manip.h
	include/rudp/sockhandling/uid_gen.h
	include/rudp/sockhandling/packets.h
	include/rudp/sockhandling/poll.h
	include/rudp/sockhandling/connstate.h
	packets.c
)

target_include_directories(rudp_sockhandling PUBLIC include)
target_link_libraries(rudp_sockhandling PUBLIC rudp_utils)

