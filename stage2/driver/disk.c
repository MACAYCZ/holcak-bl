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

	self->cylinders = ((output.ecx & UINT16_MAX) >> 0x06) + 0x01;
	self->heads = (output.edx >> 0x08) & 0x0F;
	self->sectors = output.ecx & 0x3F;

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

uint16_t disk_read(disk_t self, uint32_t address, uint16_t sectors, void *buffer)
{
	if (self.extensions & DISK_EXTENSION_PACKET_STRUCTURE) {
		disk_packet_t packet = {
			.size = sizeof(disk_packet_t),
			.reserved = 0x00,
			.sectors = sectors,
			.buffer = (uintptr_t)buffer,
			.address = address,
		};

		x86_16_regs_t input = {
			.eax = 0x4200,
			.edx = self.id,
			.ds = x86_16_SEGMENT(&packet),
			.esi = x86_16_OFFSET(&packet),
		};

		x86_16_regs_t output;
		x86_16_int(0x13, &input, &output);

		if (output.eflags & x86_FLAGS_CARRY) {
			return 0x00;
		}
		return sectors;
	}
	size_t read = 0;
	for (size_t retry = 0; read < sectors && retry < 5; read++) {
		uint16_t cylinder = (address + read) / self.sectors / self.heads;
		uint8_t head = (address + read) / self.sectors % self.heads;
		uint8_t sector = (address + read) % self.sectors + 1;

		x86_16_regs_t input = {
			.eax = 0x0200 | MIN(sectors - read, 128),
			.ebx = x86_16_OFFSET(buffer),
			.ecx = (sector & 0x3F) | (cylinder << 0x06),
			.edx = head << 0x08 | self.id,
			.es = x86_16_SEGMENT(buffer),
		};

		x86_16_regs_t output;
		x86_16_int(0x13, &input, &output);

		if (output.eflags & x86_FLAGS_CARRY || !(output.eax & 0x0F)) {
			input = (x86_16_regs_t){
				.eax = 0x00,
				.edx = self.id,
			};
			x86_16_int(0x13, &input, &output);

			if (output.eflags & x86_FLAGS_CARRY) {
				return read;
			}
			retry++;
			continue;
		}

		read += output.eax & 0x0F;
		retry = 0;
	}
	return read;
}
