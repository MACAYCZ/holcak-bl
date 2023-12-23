#pragma once
#include <stdint.h>

typedef enum {
	PMM_TYPE_USABLE = 1,
	PMM_TYPE_RESERVED,
	PMM_TYPE_ACPI_RECLAIM,
	PMM_TYPE_ACPI_NVS,
	PMM_TYPE_INVALID,
} pmm_type_t;

typedef struct __packed {
	uint64_t base;
	uint64_t size;
	uint32_t type;
	uint32_t acpi;
} pmm_node_t;

typedef struct {
	uint32_t size;
	pmm_node_t data[0xFF];
} pmm_info_t;

pmm_info_t pmm_init(void);
uint64_t pmm_alloc(uint64_t size, uint32_t type);
