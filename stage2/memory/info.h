#pragma once
#include <stdint.h>
#include "../define.h"

typedef enum {
	MEM_TYPE_USABLE = 1,
	MEM_TYPE_RESERVED,
	MEM_TYPE_ACPI_RECLAIM,
	MEM_TYPE_ACPI_NVS,
	MEM_TYPE_INVALID,
} mem_type_t;

typedef struct __packed {
	uint64_t base;
	uint64_t size;
	uint32_t type;
	uint32_t acpi;
} mem_node_t;

typedef struct {
	uint32_t size;
	uint32_t bytes;
	mem_node_t data[0xFF];
} mem_map_t;

typedef struct {
	uint16_t lower;
	uint16_t upper;
	mem_map_t map;
} mem_info_t;

mem_info_t mem_init(void);
