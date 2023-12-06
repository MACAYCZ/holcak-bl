#include <stdnoreturn.h>
#include "define.h"
#include "x86_16.h"

__cdecl noreturn void main(void)
{
	x86_16_regs_t input = {
		.eax = 0x0E30,
	};
	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);

	*((char*)0xB8000) = 'B';

	while (1);
}
