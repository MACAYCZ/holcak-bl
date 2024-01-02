#include <stage2/driver/x86_16.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

void putc(char letter)
{
	x86_16_regs_t input = {
		.eax = 0x0E00 | letter,
	};
	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);
}

void puts(const char *string)
{
	for (; *string; putc(*string++));
}

__cdecl noreturn void main(void)
{
	x86_16_regs_t input = {
		.eax = 0x0E30,
	};
	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);
	while (1);
}
