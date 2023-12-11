#pragma once
#include <stdint.h>
#include "../define.h"

typedef enum {
	MEM_TYPE_USABLE,
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


