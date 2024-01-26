#pragma once
#include <stddef.h>

void *memset(void *dst, int chr, size_t size);
void *memcpy(void *restrict dst, const void *restrict src, size_t size);
void *memmove(void *dst, const void *src, size_t size);
