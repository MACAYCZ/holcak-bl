#include "console.h"
#include "x86_16.h"

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

void putx(size_t value)
{
	for (size_t bytes = sizeof(size_t) * 2; bytes > 0; bytes--) {
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
			switch (*++fmt) {
			case 'c':
				putc(va_arg(args, int));
				break;
			case 's':
				puts(va_arg(args, const char*));
				break;
			case 'x':
				puts("0x");
				putx(va_arg(args, size_t));
				break;
			case '%':
				putc('%');
				break;
			}
		}
		else {
			putc(*fmt);
		}
	}
}
