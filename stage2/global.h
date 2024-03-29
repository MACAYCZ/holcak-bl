#pragma once
#include <stddef.h>

#define __packed       __attribute__((packed))
#define __cdecl        __attribute__((cdecl))
#define __asmcall      __cdecl extern
#define __align(X)     __attribute__((aligned(X)))
#define __interrupt    __attribute__((interrupt))
#define __maybe_unused __attribute__((unused))

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define LEN(A) (sizeof(A) / sizeof(*(A)))

typedef ptrdiff_t ssize_t;
