#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	DISK_EXTENSION_PACKET_STRUCTURE = 0x01,
	DISK_EXTENSION_LOCKING_AND_EJECTING = 0x02,
	DISK_EXTENSION_ENHANCED_DISK_DRIVE = 0x04,
} disk_extension_t;

typedef struct {
	uint8_t size;
	uint8_t reserved;
	uint16_t sectors;
	uint32_t buffer;
	uint64_t address;
} disk_packet_t;

typedef struct {
	uint8_t id;
	uint8_t extensions;
	uint16_t cylinders;
	uint8_t heads;
	uint8_t sectors;
} disk_t;

bool disk_init(disk_t *self, uint8_t id);
uint16_t disk_read(disk_t self, uint32_t address, uint16_t sectors, void *buffer);
