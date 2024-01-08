#pragma once
#include <stage2/global.h>
#include <stddef.h>
#include <stdint.h>

#define x86_16_SEGMENT(Address) ((size_t)(Address) >> 0x04)
#define x86_16_OFFSET(Address)  ((size_t)(Address) & 0x0F)

typedef enum {
	x86_FLAGS_CARRY = 0x01,
	x86_FLAGS_PARITY = 0x04,
	x86_FLAGS_HALF_CARRY = 0x10,
	x86_FLAGS_ZERO = 0x40,
	x86_FLAGS_SIGN = 0x80,
	x86_FLAGS_TRAP = 0x0100,
	x86_FLAGS_INTERRUPT = 0x0200,
	x86_FLAGS_DIRECTION = 0x0400,
	x86_FLAGS_OVERFLOW = 0x0800,
	x86_FLAGS_IO_PRIVILEGE_LEVEL = 0x3000,
	x86_FLAGS_NESTED_TASK = 0x4000,
	x86_FLAGS_MODE = 0x8000,
	x86_EFLAGS_RESUME = 0x00010000,
	x86_EFLAGS_VIRTUAL_8086 = 0x00020000,
	x86_EFLAGS_ALIGMENT_CHECK = 0x00040000,
	x86_EFLAGS_SMAP_ACCESS_CHECK = 0x00040000,
	x86_EFLAGS_VIRTUAL_INTERRUPT = 0x00080000,
	x86_EFLAGS_VIRTUAL_INTERRUPT_PENDING = 0x00100000,
	x86_EFLAGS_CPUID = 0x00200000,
	x86_EFLAGS_AES_KEY_SCHEDULE_LOADED = 0x40000000,
	x86_EFLAGS_ALTERNATE_INSTRUCTION_SET = 0x80000000,
} x86_flags_t;

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

__cdecl extern void x86_16_int(uint8_t index, x86_16_regs_t *input, x86_16_regs_t *output);
