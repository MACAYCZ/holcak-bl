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
	for (size_t i = 0; i <= UINT8_MAX; i++) {
		disk_t disk;
		if (!disk_init(&disk, i)) continue;

		putc('0' + (disk.id >> 0x04));
		putc('0' + (disk.id & 0x0F));
		putc('=');
		putc('0' + disk.extensions);
		putc('\n');
	}
	while (1);
}
