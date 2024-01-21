#include <stage2/protocol/chainload.h>
#include <stage2/interface/console.h>
#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

void puts(const char *string)
{
	for (; *string; console_print(*string++, CONSOLE_DEFAULT));
}

__cdecl noreturn void main(void)
{
	console_init();
	puts("Hello, World!\n");
	console_flush();
	while (1);

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
