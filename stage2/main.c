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

#define MENU_DEFAULT_NAME "Unknown"

typedef struct {
	struct {
		disk_t disk;
		char name[13];
	} items[16];
	size_t size;
	size_t cursor;
	size_t render;
} menu_t;

__cdecl noreturn void main(__maybe_unused uint8_t boot)
{
	console_init();

	static menu_t menu = {};
	for (size_t i = 0; i < 0xFF; i++) {
		if (menu.size == LEN(menu.items)) {
			printf("ERROR: Number of bootable devices exceeded the maximum!\n");
			break;
		}
		disk_t *disk = &menu.items[menu.size].disk;
		if (!disk_init(disk, i)) {
			continue;
		}
		if (boot != i && chainload_init(*disk)) {
			if (!chainload_name(menu.items[menu.size].name)) {
				memcpy(menu.items[menu.size].name, MENU_DEFAULT_NAME, LEN(MENU_DEFAULT_NAME));
			}
			menu.size++;
		}
	}
	if (!menu.size) {
		printf("ERROR: Could not find any bootable device!\n");
		while (1);
	}
//	if (menu.size == 1) {
//		chainload_init(menu.items[0].disk);
//		chainload_jump(menu.items[0].disk.id);
//	}

	while (1) {
		console_clear();
		for (size_t i = menu.render; i < MIN(menu.size, menu.render + CONSOLE_ROWS); i++) {
			printf("%.13s\n", menu.items[i].name);
		}
		console_cursor = (menu.cursor - menu.render) * CONSOLE_COLS;
		console_flush();
	keyboard:
		uint8_t key;
		switch (port8_in(0x60)) {
		case 0xE0:
			while ((key = port8_in(0x60)) == 0xE0);
			switch (key) {
			case 0x48:
				if (menu.cursor) menu.cursor--;
				if (menu.cursor < menu.render) menu.render--;
				break;
			case 0x50:
				if (menu.cursor < menu.size - 1) menu.cursor++;
				if (menu.cursor > menu.render + CONSOLE_ROWS) menu.render++;
				break;
			default:
				goto keyboard;
			}
			break;
		case 0x1C:
			while (port8_in(0x60) == 0x1C);
			console_clear();
			chainload_init(menu.items[menu.cursor].disk);
			chainload_jump(menu.items[menu.cursor].disk.id);
		default:
			goto keyboard;
		}
	}
}
