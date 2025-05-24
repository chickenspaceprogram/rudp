# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.


cmake_minimum_required(VERSION 3.12)
project(user_rudp C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# this file does kinda suck but it also does work and i can't be bothered to make it decent

if(DEFINED RUDP_USE_ASAN AND RUDP_USE_ASAN)
	message(NOTICE "Option `RUDP_USE_ASAN` specified. Linking ASAN and setting `RUDP_DEBUG`.")
	set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize-recover=address -fsanitize=undefined -fsanitize=leak")
	set(RUDP_DEBUG ON)
endif()

if (DEFINED RUDP_DEBUG AND RUDP_DEBUG)
	message(NOTICE "Option `RUDP_DEBUG` specified. Building for debug and adding extra compile flags.")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
	set(CMAKE_BUILD_TYPE Debug)
endif()

if (DEFINED RUDP_CPPCHECK AND RUDP_CPPCHECK)
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

if (DEFINED RUDP_CLANG_TIDY AND RUDP_CLANG_TIDY)
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

add_subdirectory(src)

