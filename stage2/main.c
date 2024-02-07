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

typedef struct {
	struct {
		disk_t disk;
	} items[16];
	size_t size;
	size_t cursor;
	size_t render;
} menu_t;

__cdecl noreturn void main(void)
{
	console_init();
//	static menu_t menu;

	/*
	printf("String %10.*s\n", 5, "World");
	printf("Dec    %10.5d\n", 1234);
	printf("Hex    %10.5x\n", 0x1234);
	printf("Oct    %10.5o\n", 01234);
	printf("Zero   %10.0d\n", 0);
	printf("Zero   %10.1d\n", 0);
	while (1);
	*/

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
	if (!disk_init(&disk, 0x00)) {
		printf("ERROR: Could not initialize disk!");
		while (1);
	}

	if (!chainload_init(disk)) {
		printf("ERROR: Could not chainload!");
		while (1);
	}

	chainload_jump(disk.id);
}
