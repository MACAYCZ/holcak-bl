#pragma once
#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include <stdnoreturn.h>
#include <stdbool.h>
#include <stdint.h>

#define CHAINLOAD_SIGNATURE 0xAA55

bool chainload_load(disk_t disk);
__cdecl noreturn extern void chainload_jump(uint8_t disk);
