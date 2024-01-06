#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include "chainload.h"

noreturn __cdecl extern void chainload_jump(uint8_t disk);

void chainload(disk_t disk)
{
	if (!disk_read(disk, 0, 1, (void*)0x7C00)) {
		return;
	}
	if (*(uint16_t*)0x7DFE != CHAINLOAD_SIGNATURE) {
		return;
	}
	chainload_jump(disk.id);
}
