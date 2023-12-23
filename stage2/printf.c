#include <stage2/driver/x86_16.h>
#include <stdint.h>
#include "printf.h"

void putc(char chr)
{
	x86_16_regs_t input = {
		.eax = 0x0E00 | chr,
	};

	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);
}

void puts(const char *str)
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
		char chr = ((value >> ((bytes - 1) << 2)) & 0x0F) + '0';
		putc(chr > '9' ? chr + 0x07 : chr);
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
