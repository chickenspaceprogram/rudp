# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)

add_library(rudp_timers
	include/rudp/timers/timespec_manip.h
)

target_include_directories(rudp_timers PUBLIC include)
target_link_libraries(rudp_timers PUBLIC rudp_utils)

