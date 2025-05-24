cmake_minimum_required(VERSION 3.12)

add_library(rudp_mem 
	alloc.c
	include/rudp/alloc.h
)
target_include_directories(rudp_mem PUBLIC include)


