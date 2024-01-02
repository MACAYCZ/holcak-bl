#pragma once
#include <stdbool.h>
#include <stdint.h>

// TODO: Increase maximum number of sectors that `disk_read` can read.

bool disk_init(uint8_t disk);
uint64_t disk_read(uint8_t disk, uint64_t address, uint16_t sectors, void *buffer);
