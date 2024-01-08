#include <stage2/protocol/chainload.h>
#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
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
	puts("Hello, World!\n\r");

	disk_t disk;
	if (!disk_init(&disk, 0x00)) {
		puts("ERROR: Could not initialize disk!");
		while (1);
	}

	if (!chainload_load(disk)) {
		puts("ERROR: Could not chainload!");
		while (1);
	}

	chainload_jump(disk.id);
}
