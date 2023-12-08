#include <stdnoreturn.h>
#include "define.h"
#include "x86_16.h"

typedef struct {
	uint64_t base;
	uint64_t size;
	uint32_t type;
	uint32_t acpi;
} x86_memory_map_t;

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
	for (size_t i = 0; str[i]; i++)
	{
		x86_putc(str[i]);
	}
}

__cdecl noreturn void main(void)
{
	/*
	x86_memory_map_t memory;

	x86_16_regs_t input = {
		.eax = 0xE820,
		.edx = 0x534D4150,
		.ebx = 0x00,
		.ecx = 0x14,
		.es = x86_16_SEGMENT(&memory),
		.edi = x86_16_OFFSET(&memory),
	};

	x86_16_regs_t output;
	x86_16_int(0x15, &input, &output);
	*/

	x86_puts("Hello, World!");

	while (1);
}
