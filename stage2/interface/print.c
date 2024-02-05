#include <stage2/interface/console.h>
#include <stage2/library/string.h>
#include <stage2/library/ctype.h>
#include <stage2/global.h>
#include <stdbool.h>
#include "print.h"

// TODO: Add to standard headers: https://en.cppreference.com/w/c/string/byte/strtoimax
static size_t s2u(const char **str)
{
	size_t result = 0;
	while (isdigit(**str)) {
		result = result * 10 + *(*str)++ - '0';
	}
	return result;
}

void putc(char chr)
{
	console_write(chr);
	console_flush();
}

void puts(const char *str)
{
	while (*str) {
		console_write(*str++);
	}
	console_flush();
}

void printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void vprintf(const char *fmt, va_list args)
{
	while (*fmt) {
		if (*fmt != '%') {
			console_write(*fmt++);
			continue;
		}
		fmt++;
		// TODO: Flags
		size_t width = 0;
		switch (*fmt) {
		case '*':
			width = va_arg(args, size_t);
			fmt++;
			break;
		default:
			if (isdigit(*fmt)) width = s2u(&fmt);
			break;
		}
		size_t precision = SIZE_MAX;
		if (*fmt == '.') {
			switch (*++fmt) {
			case '*':
				precision = va_arg(args, size_t);
				fmt++;
				break;
			default:
				if (isdigit(*fmt)) precision = s2u(&fmt);
				break;
			}
		}
		// TODO: Length
		switch (*fmt) {
		case 'c':
			console_write(va_arg(args, int));
			break;
		case 's': {
			const char *str = va_arg(args, const char*);
			size_t size = strnlen_s(str, precision);

			if (width > size) {
				for (size_t i = 0; i < width - size; i++, console_write(' '));
			}
			for (size_t i = 0; i < size; console_write(str[i++]));
			break;
		}
		default:
			console_write('%');
			break;
		}
		fmt++;
	}
	console_flush();
}
