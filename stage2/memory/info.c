#include "../console.h"
#include "../x86_16.h"
#include "info.h"

mem_info_t mem_init(void)
{
	mem_info_t self;
	x86_16_regs_t input, output;

	input = (x86_16_regs_t){.eax = 0xE801};
	x86_16_int(0x15, &input, &output);

	uint8_t error = output.eax >> 0x04;
	if (error == 0x80 || error == 0x86 || output.eflags & x86_EFLAGS_CARRY) {
		printf("[ERROR] Detecting memory failed!\n\r");
		while (1);
	}

	self.lower = output.eax ? output.eax : output.ecx;
	self.upper = output.ebx ? output.ebx : output.edx;

	// TODO: Convert upper memory to K instead of 64K!
	printf("[DEBUG] Detected memory:\n\r    Lower: %x\n\r    Upper: %x\n\r", self.lower, self.upper);

	return self;
}
