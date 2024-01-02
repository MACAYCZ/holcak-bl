#include <stage2/driver/x86_16.h>
#include "disk.h"

bool disk_init(disk_t *self, uint8_t id)
{
	x86_16_regs_t input = {
		.eax = 0x0800,
		.edx = id,
	};

	x86_16_regs_t output = {0};
	x86_16_int(0x13, &input, &output);

	if (output.eflags & x86_FLAGS_CARRY) {
		return false;
	}

	input = (x86_16_regs_t){
		.eax = 0x4100,
		.ebx = 0x55AA,
		.edx = id,
	};

	output.eflags = 0x00;
	x86_16_int(0x13, &input, &output);

	self->id = id;
	self->extensions = (output.eflags & x86_FLAGS_CARRY) ? 0x00 : output.ecx;
	return true;
}

void disk_read(disk_t self, uint32_t base, uint16_t size, void *buffer)
{
}
