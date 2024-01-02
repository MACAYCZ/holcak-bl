#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
#include <stage2/global.h>

typedef struct __packed {
	uint8_t size;
	uint8_t reserved;
	uint16_t sectors;
	uint32_t buffer;
	uint64_t address;
} disk_packet_t;

bool disk_init(uint8_t disk)
{
	x86_16_regs_t input = {
		.eax = 0x0800,
		.edx = disk,
	};

	x86_16_regs_t output;
	x86_16_int(0x13, &input, &output);

	return ~output.eflags & x86_EFLAGS_CARRY;
}

uint64_t disk_read(uint8_t disk, uint64_t address, uint16_t sectors, void *buffer)
{
	// TODO: Save extensions durning initialization!
	x86_16_regs_t input = {
		.eax = 0x4100,
		.ebx = 0x55AA,
		.edx = disk,
	};

	x86_16_regs_t output;
	x86_16_int(0x13, &input, &output);

	if (output.eflags & x86_EFLAGS_CARRY || output.ebx != 0xAA55) {
		return 0;
	}

	if (output.ecx & 0x01) {
		disk_packet_t packet = {
			.size = 0x10,
			.reserved = 0x00,
			.sectors = sectors,
			.buffer = (uint32_t)buffer,
			.address = address,
		};

		input = (x86_16_regs_t){
			.eax = 0x4200,
			.edx = disk,
			.ds = x86_16_SEGMENT(&packet),
			.esi = x86_16_OFFSET(&packet),
		};

		x86_16_int(0x13, &input, &output);

		if (output.eflags & x86_EFLAGS_CARRY) {
			return 0;
		}
		return sectors;
	} else for (size_t read = 0; read < sectors; ) {
		input = (x86_16_regs_t){
			.eax = 0x0200 | (sectors & 0xFF),
			.ebx = x86_16_OFFSET(buffer),
			// TODO: https://stanislavs.org/helppc/int_13-2.html
		};

		// TODO!
//		return sectors - read;
	}
	__builtin_unreachable();
}
