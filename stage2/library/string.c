#include "string.h"

char *strcpy(char *restrict dst, const char *restrict src) {
	for (size_t i = 0; (dst[i] = src[i]); i++);
	return dst;
}

size_t strlen(const char *str) {
	size_t i;
	for (i = 0; str[i]; i++);
	return i;
}

size_t strnlen_s(const char *str, size_t size)
{
	size_t i;
	for (i = 0; str[i] && i < size; i++);
	return i;
}

void *memset(void *dst, int chr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		((char*)dst)[i] = chr;
	}
	return dst;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t size) {
	for (size_t i = 0; i < size; i++) {
		((char*)dst)[i] = ((char*)src)[i];
	}
	return dst;
}

void *memmove(void *dst, const void *src, size_t size) {
	if (dst < src) {
		return memcpy(dst, src, size);
	}
	for (size_t i = size; i > 0; i--) {
		((char*)dst)[i-1] = ((char*)src)[i-1];
	}
	return dst;
}
