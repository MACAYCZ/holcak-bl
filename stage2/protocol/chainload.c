#include <stage2/library/string.h>
#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include "chainload.h"

bool chainload_init(disk_t disk)
{
	if (!disk_read(disk, 0, 1, (void*)0x7C00)) {
		return false;
	}
	if (*(uint16_t*)0x7DFE != CHAINLOAD_SIGNATURE) {
		return false;
	}
	return true;
}

bool chainload_name(char buffer[11])
{
	if (*(uint8_t*)0x7C26 == 0x29) {
		memcpy(buffer, (char*)0x7C2B, 11);
		return true;
	}
	if (*(uint8_t*)0x7C42 == 0x29) {
		memcpy(buffer, (char*)0x7C47, 11);
		return true;
	}
	return false;
}
