#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "pata.h"

typedef enum {
	DISK_TYPE_PATA,
} disk_type_t;

typedef union {
	pata_t pata;
} disk_data_t;

typedef struct {
	disk_type_t type;
	disk_data_t data;
	uint64_t begin;
	uint64_t limit;
} disk_t;

disk_t disk_pata(uint16_t base, bool master);
uint32_t disk_read(disk_t *self, uint64_t begin, void *buffer, uint32_t limit);
uint32_t disk_write(disk_t *self, uint64_t begin, const void *buffer, uint32_t limit);
