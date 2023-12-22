#include <stage2/driver/x86_16.h>
#include <stage2/global.h>
#include <stage2/printf.h>
#include <stdnoreturn.h>

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

__cdecl noreturn void main(void)
{
	puts("Hello, World!\n\r");
	while (1);
}
