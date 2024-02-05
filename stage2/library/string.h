#pragma once
#include <stdint.h>
#include <stddef.h>

char *strcpy(char *restrict dst, const char *restrict src);
size_t strlen(const char *str);
size_t strnlen_s(const char *str, size_t size);

void *memset(void *dst, int chr, size_t size);
void *memcpy(void *restrict dst, const void *restrict src, size_t size);
void *memmove(void *dst, const void *src, size_t size);
