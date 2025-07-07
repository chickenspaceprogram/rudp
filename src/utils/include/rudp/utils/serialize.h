#pragma once
#include <limits.h>
#include <stddef.h>
#include <stdint.h>


#define rudp_serialize_int(BUFPTR, NUM) do {\
	for (size_t i = 1; i <= sizeof(NUM); ++i) {\
		(BUFPTR)[i - 1] = ((NUM) & ((uintmax_t)0xFF << (sizeof(NUM) - i) * (CHAR_BIT))) >> ((sizeof(NUM) - i) * (CHAR_BIT));\
	}\
} while (0)

