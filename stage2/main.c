#include <stdnoreturn.h>
#include "define.h"
#include "x86_16.h"

void x86_putc(char chr)
{
	x86_16_regs_t input = {
		.eax = 0x0E00 | chr,
	};

	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);
}

void x86_puts(const char *str)
{
	for (size_t i = 0; str[i]; i++) {
		x86_putc(str[i]);
	}
}

__cdecl noreturn void main(void)
{
	x86_puts("Hello, World!\n\r");
	x86_puts("Hello, World!\n\r");
	while (1);
}
