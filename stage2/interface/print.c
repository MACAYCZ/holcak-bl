#include <stdint.h>
#include "print.h"

static void putc(char letter, uint8_t color, uint16_t *cursor)
{
	if (letter == '\n') {
		if (*cursor >= (CONSOLE_ROWS - 1) * CONSOLE_COLS) {
			*cursor = (CONSOLE_ROWS - 1) * CONSOLE_COLS;
			console_scroll(1);
		}
		else {
			*cursor = (*cursor + CONSOLE_COLS - 1) / CONSOLE_COLS * CONSOLE_COLS;
		}
	}
	else {
		CONSOLE_TEXT[*cursor] = (color << 0x08) | letter;
		*cursor += 1;
	}
}

static void puts(const char *str, uint8_t color, uint16_t *cursor)
{
	for (; *str; putc(*str++, color, cursor));
}

static void putu(uint64_t value, uint8_t base, uint8_t color, uint16_t *cursor)
{
	char buffer[sizeof(value) * 8 - 1];
	buffer[sizeof(buffer) - 1] = '\0';

	size_t i = 1;
	do {
		uint64_t reminder = value % base;
		buffer[sizeof(buffer) - ++i] = reminder + (reminder <= 9 ? '0' : 'A' - 10);
	} while (value /= base);

	puts(&buffer[sizeof(buffer) - i], color, cursor);
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
	uint8_t color = CONSOLE_DEFAULT;
	uint16_t cursor = console_cursor_get();

	for (; *fmt; fmt++) {
		if (*fmt == '\e') {
			color = *++fmt;
			continue;
		}
		else if (*fmt != '%') {
			putc(*fmt, color, &cursor);
			continue;
		}

		size_t bytes = 0;

	format:
		switch (*++fmt) {
		case 'c':
			putc(va_arg(args, int), color, &cursor);
			break;
		case 's':
			puts(va_arg(args, const char*), color, &cursor);
			break;
		case 'h':
			bytes = bytes ? bytes >> 1 : sizeof(uint16_t);
			goto format;
		case 'l':
			bytes = bytes ? bytes << 1 : sizeof(uint32_t);
			goto format;
		case 'o':
			bytes = bytes ? bytes : sizeof(uint32_t);
			putu((bytes >= sizeof(uint64_t) ? va_arg(args, uint64_t) : va_arg(args, uint32_t) & (((uint64_t)1 << (bytes << 3)) - 1)), 8, color, &cursor);
			break;
		case 'u':
			bytes = bytes ? bytes : sizeof(uint32_t);
			putu((bytes >= sizeof(uint64_t) ? va_arg(args, uint64_t) : va_arg(args, uint32_t) & (((uint64_t)1 << (bytes << 3)) - 1)), 10, color, &cursor);
			break;
		case 'x':
		case 'p':
			bytes = bytes ? bytes : sizeof(uint32_t);
			putu((bytes >= sizeof(uint64_t) ? va_arg(args, uint64_t) : va_arg(args, uint32_t) & (((uint64_t)1 << (bytes << 3)) - 1)), 16, color, &cursor);
			break;
		case '%':
			putc('%', color, &cursor);
			break;
		default:
			putc('%', color, &cursor);
			putc(*fmt, color, &cursor);
		}
	}

	console_cursor_set(cursor);
}

/*
static void putc(char letter)
{
	x86_16_regs_t input = {
		.eax = 0x0E00 | letter,
	};

	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);
}

static void puts(const char *str)
{
	for (size_t i = 0; str[i]; i++) {
		putc(str[i]);
	}
}

void putx(uint64_t value, size_t bytes)
{
	puts("0x");
	bytes <<= 1;
	for (; bytes > 0; bytes--) {
		char letter = ((value >> ((bytes - 1) << 2)) & 0x0F) + '0';
		putc(letter > '9' ? letter + 0x07 : letter);
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
	for (; *fmt; fmt++) {
		if (*fmt == '%') {
			size_t bytes = sizeof(uint32_t);
		format:
			switch (*++fmt) {
			case 'c':
				putc(va_arg(args, int));
				break;
			case 's':
				puts(va_arg(args, const char*));
				break;
			case 'h':
				bytes >>= 1;
				goto format;
			case 'l':
				bytes <<= 1;
				goto format;
			case 'x': {
				uint64_t value = (bytes >= sizeof(uint64_t) ? va_arg(args, uint64_t) : va_arg(args, uint32_t));
				putx(value, MIN(bytes, sizeof(uint64_t)));
			} break;
			case '%':
				putc('%');
				break;
			}
		}
		else if (*fmt == '\n') {
			puts("\n\r");
		}
		else if (*fmt == '\t') {
			for (size_t i = 0; i < 4; i++) {
				putc(' ');
			}
		}
		else {
			putc(*fmt);
		}
	}
}
*/
