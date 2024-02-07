#include <stage2/interface/console.h>
#include <stage2/library/string.h>
#include <stage2/library/ctype.h>
#include <stage2/global.h>
#include <stdbool.h>
#include "print.h"

#define putc(Value) console_write(Value)

// TODO: Add to standard headers: https://en.cppreference.com/w/c/string/byte/strtoimax
static size_t s2u(const char **str)
{
	size_t result = 0;
	while (isdigit(**str)) {
		result = result * 10 + *(*str)++ - '0';
	}
	return result;
}

static void puts(const char *str, size_t width, size_t precision)
{
	size_t size = strnlen_s(str, precision);
	if (width > size) {
		for (size_t i = 0; i < width - size; i++, putc(' '));
	}
	for (size_t i = 0; i < size; putc(str[i++]));
}

// TODO: Functions `puti` and `putu` are really similar
static void puti(ssize_t value, size_t width, size_t precision, size_t base)
{
	if (precision == SIZE_MAX) {
		precision = 1;
	}
	if (value || precision) {
		char buffer[11];
		strfi(buffer, value, base);
		size_t size = strlen(buffer);
		if (width > precision) {
			for (size_t i = 0; i < width - precision; i++, putc(' '));
		}
		for (; size < precision; size++, putc('0'));
		for (size_t i = 0; i < size; putc(buffer[i++]));
	}
}

static void putu(size_t value, size_t width, size_t precision, size_t base)
{
	if (precision == SIZE_MAX) {
		precision = 1;
	}
	if (value || precision) {
		char buffer[11];
		strfu(buffer, value, base);
		size_t size = strlen(buffer);
		if (width > precision) {
			for (size_t i = 0; i < width - precision; i++, putc(' '));
		}
		for (; size < precision; size++, putc('0'));
		for (size_t i = 0; i < size; putc(buffer[i++]));
	}
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
			putc(*fmt++);
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
			putc(va_arg(args, int));
			break;
		case 's':
			puts(va_arg(args, char*), width, precision);
			break;
		case 'd':
			// TODO: Change to length specified type!
			puti(va_arg(args, ssize_t), width, precision, 10);
			break;
		case 'u':
			// TODO: Change to length specified type!
			putu(va_arg(args, size_t), width, precision, 10);
			break;
		// TODO: Support lower hexadecimal!
		case 'o':
			putu(va_arg(args, size_t), width, precision, 8);
			break;
		case 'x':
		case 'X':
			// TODO: Change to length specified type!
			putu(va_arg(args, size_t), width, precision, 16);
			break;
		default:
			putc('%');
			break;
		}
		fmt++;
	}
	console_flush();
}
