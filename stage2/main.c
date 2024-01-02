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
	const uint8_t disk = 0x80;

	if (!disk_init(disk)) {
		puts("Error: Initializing disk failed!\n\r");
		while (1);
	}

	uint8_t buffer[512];
	const uint16_t sectors = sizeof(buffer) / 512;

	if (disk_read(disk, 0, sectors, buffer) != sectors) {
		puts("Error: Reading from disk failed!\n\r");
		while (1);
	}

	puts(&buffer[3]);

	while (1);
}
