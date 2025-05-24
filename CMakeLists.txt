cmake_minimum_required(VERSION 3.12)
project(user_rudp C)

if (NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if(DEFINED RUDP_USE_ASAN AND RUDP_USE_ASAN)
	message(NOTICE "Option `RUDP_USE_ASAN` specified. Linking ASAN.")
	set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize-recover=address -fsanitize=undefined -fsanitize=leak")
endif()

if (DEFINED RUDP_DEBUG AND RUDP_DEBUG)
	message(NOTICE "Option `RUDP_DEBUG` specified. Building for debug and adding extra compile flags.")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
	set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(src)

