#pragma once
#include <stage2/driver/disk.h>
#include <stage2/global.h>
#include <stdnoreturn.h>
#include <stdbool.h>
#include <stdint.h>

// TODO: Structure representing the boot sector!
// TODO: Define BDB signatures

#define CHAINLOAD_SIGNATURE 0xAA55

bool chainload_init(disk_t disk);
bool chainload_name(char buffer[11]);
__cdecl noreturn extern void chainload_jump(uint8_t disk);
