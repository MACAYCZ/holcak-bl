#include <stage2/library/ctype.h>
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

void strrev(char *str)
{
	strnrev_s(str, strlen(str));
}

void strnrev_s(char *str, size_t size)
{
	for (size_t i = 0; i <= size / 2; i++) {
		char value = str[i];
		str[i] = str[size-i];
		str[size-i] = value;
	}
}

void strfi(char *buffer, ssize_t value, size_t base)
{
	if (value < 0) {
		*buffer++ = '-';
		value = -value;
	}
	strfu(buffer, value, base);
}

void strfu(char *buffer, size_t value, size_t base)
{
	size_t size;
	for (size = 0; !size || (value /= base); size++) {
		size_t reminder = value % base;
		buffer[size] = reminder + (reminder <= 9 ? '0' : 'A' - 10);
	}
	buffer[size] = '\0';
	strnrev_s(buffer, size-1);
}

ssize_t strti(const char **buffer, size_t base)
{
	if (**buffer == '-') {
		(*buffer)++;
		return -strtu(buffer, base);
	}
	return strtu(buffer, base);
}

size_t strtu(const char **buffer, size_t base)
{
	size_t result = 0;
	for (size_t value; (value = **buffer); (*buffer)++) {
		if (isdigit(value)) value -= '0';
		else if (islower(tolower(value))) value -= 'a' - 0x10;
		else break;
		if (value >= base) break;
		result = result * base + value;
	}
	return result;
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
