#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	DISK_EXTENSION_PACKET_STRUCTURE = 0x01,
	DISK_EXTENSION_LOCKING_AND_EJECTING = 0x02,
	DISK_EXTENSION_ENHANCED_DISK_DRIVE = 0x04,
} disk_extension_t;

typedef struct {
	uint8_t id;
	uint8_t extensions;
} disk_t;

bool disk_init(disk_t *self, uint8_t id);
void disk_read(disk_t self, uint32_t base, uint16_t size, void *buffer);
