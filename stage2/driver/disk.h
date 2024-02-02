#pragma once
#include <stdbool.h>
#include <stdint.h>

#define DISK_EXTENSION_PACKET_STRUCTURE     0x01
#define DISK_EXTENSION_LOCKING_AND_EJECTING 0x02
#define DISK_EXTENSION_ENHANCED_DISK_DRIVE  0x04

typedef struct {
	uint8_t id;
	uint8_t extensions;
	uint16_t cylinders;
	uint8_t heads;
	uint8_t sectors;
} disk_t;

bool disk_init(disk_t *self, uint8_t id);
uint16_t disk_read(disk_t self, uint32_t address, uint16_t sectors, void *buffer);
