#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include "chainload.h"

bool chainload_load(disk_t disk)
{
	if (!disk_read(disk, 0, 1, (void*)0x7C00)) {
		return false;
	}
	if (*(uint16_t*)0x7DFE != CHAINLOAD_SIGNATURE) {
		return false;
	}
	return true;
}
