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
	disk_t disk;
	if (!disk_init(&disk, 0x80)) {
		puts("Error: Could not initialize disk!");
		while (1);
	}

	char buffer[512];
	if (!disk_read(disk, 0, 1, buffer)) {
		puts("Error: Could not read from disk!");
		while (1);
	}

	puts(&buffer[3]);
	while (1);
}
