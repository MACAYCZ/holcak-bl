#pragma once
#include <stage2/global.h>
#include <stdint.h>
#include <stddef.h>

#define x86_16_SEGMENT(Address) ((size_t)(Address) >> 0x04)
#define x86_16_OFFSET(Address) ((size_t)(Address) & 0x0F)

typedef enum {
	x86_EFLAGS_CARRY = 0x01,
} x86_eflags_t;

typedef struct __packed {
	uint16_t gs;
	uint16_t fs;
	uint16_t es;
	uint16_t ds;
	uint32_t eflags;
	uint32_t ebp;
	uint32_t edi;
	uint32_t esi;
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;
} x86_16_regs_t;

__cdecl extern void x86_16_int(uint8_t index, const x86_16_regs_t *input, x86_16_regs_t *output);
