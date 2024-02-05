#include <stage2/protocol/chainload.h>
#include <stage2/interface/console.h>
#include <stage2/interface/print.h>
#include <stage2/library/string.h>
#include <stage2/library/ctype.h>
#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
#include <stage2/driver/port.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

__cdecl noreturn void main(void)
{
	console_init();
	puts("Hello, World!\n");

//	printf("Hello, %4.*s!\n", 5, "World");
//	while (1);

/*
	while (1) {
		switch (port8_in(0x60)) {
		case 0xE0:
			switch (port8_in(0x60)) {
			case 0x48:
				puts("Arrow up!\n");
				break;
			case 0x50:
				puts("Arrow down!\n");
				break;
			}
			break;
		case 0x1C:
			puts("Enter!\n");
			break;
		}
	}
*/

	disk_t disk;
	if (!disk_init(&disk, 0x80)) {
		puts("ERROR: Could not initialize disk!");
		while (1);
	}

	if (!chainload_init(disk)) {
		puts("ERROR: Could not chainload!");
		while (1);
	}

	chainload_jump(disk.id);
}
