#pragma once
#include <stage2/global.h>
#include <stdint.h>

typedef struct {
	uint16_t lower;
	uint16_t upper;
} mem_info_t;

mem_info_t mem_init(void);
