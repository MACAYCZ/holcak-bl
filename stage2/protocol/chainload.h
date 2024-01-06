#pragma once
#include <stage2/driver/disk.h>
#include <stdnoreturn.h>
#include <stdbool.h>
#include <stdint.h>

#define CHAINLOAD_SIGNATURE 0xAA55

void chainload(disk_t disk);
