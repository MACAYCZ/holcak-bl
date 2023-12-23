#include <stage2/driver/x86_16.h>
#include <stage2/printf.h>
#include "info.h"

mem_info_t mem_init(void)
{
	mem_info_t self;
	x86_16_regs_t input, output;

	input.eax = 0xE801;
	x86_16_int(0x15, &input, &output);

	if (output.eflags & x86_EFLAGS_CARRY || (output.eax & 0xFF00) == 0x8000 || (output.eax & 0xFF00) == 0x8600) {
		printf("[ERROR] Detecting memory failed!\n");
		while (1);
	}

	if (output.ecx) {
		output.eax = output.ecx;
		output.ebx = output.edx;
	}

	self.lower = output.eax;
	self.upper = output.ebx;
	printf("[DEBUG] Memory detected!\n\tLower: %hx\n\tUpper: %hx\n", self.lower, self.upper);

	return self;
}
