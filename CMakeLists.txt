# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.


cmake_minimum_required(VERSION 3.14)
project(user_rudp C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# this file does kinda suck but it also does work and i can't be bothered to make it decent

option(RUDP_USE_ASAN "Links rudp and any of its tests with the ASAN library.")
option(RUDP_DEBUG "Builds a debug version of rudp and adds extra GCC/Clang-specific compiler flags.")
option(RUDP_CPPCHECK "Uses the cppcheck tool to statically analyze source code when building.")
option(RUDP_CLANG_TIDY "Uses the clang-tidy tool to statically analyze source code when building.")
option(RUDP_TESTS "Builds rudp's unit tests.")

if(RUDP_TESTS)
	include(CTest)
endif()


if(RUDP_USE_ASAN)
	message(NOTICE "Option `RUDP_USE_ASAN` specified. Linking ASAN and setting `RUDP_DEBUG`.")
	set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize-recover=address -fsanitize=undefined -fsanitize=leak")
	set(RUDP_DEBUG ON)
endif()

if (RUDP_DEBUG)
	message(NOTICE "Option `RUDP_DEBUG` specified. Building for debug and adding extra compile flags.")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (RUDP_CPPCHECK)
	find_program(CPPCHECK cppcheck)
	if (CPPCHECK)
		message(NOTICE "Found cppcheck!")
		list(APPEND CMAKE_CXX_CPPCHECK
			"--enable=all"
			"--inconclusive"
			"--force"
			"--std=c11"
		)
	else()
		message(WARNING "Failed to find cppcheck. Continuing...")
	endif()
endif()

if (RUDP_CLANG_TIDY)
	find_program(CLANG_TIDY clang-tidy)
	if (CLANG_TIDY)
		message(NOTICE "Found clang-tidy!")
		list(APPEND CMAKE_C_CLANG_TIDY
			"--use-color"
		)
		set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY})
	else()
		message(NOTICE "clang-tidy not found and not used.")
	endif()
endif()



function(make_test testfilename) # can take libs to link as an optional arg
if (RUDP_TESTS)
	get_filename_component(EXENAME ${testfilename} NAME_WLE)
	add_executable(${EXENAME} ${testfilename})
	target_link_libraries(${EXENAME} ${ARGN})
	add_test(
		NAME ${EXENAME}
		COMMAND $<TARGET_FILE:${EXENAME}>
	)
endif()
endfunction()

include(CheckSymbolExists)

check_symbol_exists(epoll_create "sys/epoll.h" HAVE_EPOLL_CREATE)
check_symbol_exists(epoll_ctl "sys/epoll.h" HAVE_EPOLL_CTL)
check_symbol_exists(epoll_wait "sys/epoll.h" HAVE_EPOLL_WAIT)

check_symbol_exists(kqueue "sys/event.h" HAVE_KQUEUE_KQUEUE)
check_symbol_exists(kevent "sys/event.h" HAVE_KQUEUE_KEVENT)
check_symbol_exists(EV_SET "sys/event.h" HAVE_KQUEUE_EV_SET)

if (HAVE_EPOLL_CREATE AND HAVE_EPOLL_CTL AND HAVE_EPOLL_WAIT)
	message(NOTICE "epoll backend found!")
	set(HAVE_EPOLL ON)
else()
	message(NOTICE "epoll backend not found")
	set(HAVE_EPOLL OFF)
endif()

if (HAVE_KQUEUE_KQUEUE AND HAVE_KQUEUE_KEVENT AND HAVE_KQUEUE_EV_SET)
	message(NOTICE "kqueue backend found!")
	set(HAVE_KQUEUE ON)
else()
	message(NOTICE "kqueue backend not found")
	set(HAVE_KQUEUE OFF)
endif()



add_subdirectory(src)




