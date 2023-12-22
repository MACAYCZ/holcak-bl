#pragma once
#include <stdint.h>

#define __packed __attribute__((packed))
#define __cdecl  __attribute__((cdecl))

#ifndef _MSC_VER
#	define __align(B) __attribute__((aligned(B)))
#else //_MSC_VER
#	define __align(B) __declspec(align(B))
#endif//_MSC_VER

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
