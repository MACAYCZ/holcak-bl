#include <stage2/protocol/chainload.h>
#include <stage2/interface/console.h>
#include <stage2/library/string.h>
#include <stage2/driver/x86_16.h>
#include <stage2/driver/disk.h>
#include <stage2/driver/port.h>
#include <stage2/global.h>
#include <stdnoreturn.h>

typedef struct {
	struct {
		disk_t disk;
		char name[13];
	} items[16];
	size_t size;
	size_t render;
	size_t cursor;
} menu_t;

// TODO: `console_init` does not return on real hardware!

__cdecl noreturn void main(void)
{
	console_init();
	puts("Hello, World!\n");
	while (1);

	/*
	console_init();
	puts("Hello, World!\n");
	while (1);

	menu_t menu = {0};
	for (size_t i = 0x00; i < 0xFF; i++) {
		if (!disk_init(&menu.items[menu.size].disk, i)) {
			continue;
		}
		if (chainload_init(menu.items[menu.size].disk)) {
			if (!chainload_name(menu.items[menu.size].name)) {
				memcpy(menu.items[menu.size].name, "Unknown", 7);
			}
			if (++menu.size > sizeof(menu.items) / sizeof(menu.items[0])) {
				puts("ERROR: Unable to load all bootable devices!\n");
				goto skip;
			}
		}
	}
skip:

	for (size_t i = 0; i < menu.size; i++) {
		for (size_t j = 0; j < 13; putc(menu.items[i].name[j++]));
		putc('\n');
		console_flush();
	}

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
		puts("ERROR: Could not initialize disk!");
		while (1);
	}

	if (!chainload_init(disk)) {
		puts("ERROR: Could not chainload!");
		while (1);
	}

	chainload_jump(disk.id);
}
