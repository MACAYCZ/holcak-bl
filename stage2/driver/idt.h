#pragma once
#include <stage2/global.h>
#include <stdbool.h>
#include <stdint.h>

#define IDT_GATE_TASK    0x05
#define IDT_GATE_INT     0x0E
#define IDT_GATE_TRAP    0x0F
#define IDT_GATE_RING0   0x00
#define IDT_GATE_RING1   0x20
#define IDT_GATE_RING2   0x40
#define IDT_GATE_RING3   0x60
#define IDT_GATE_PRESENT 0x80

typedef struct __packed {
	uint16_t base_lo;
	uint16_t segment;
	uint8_t  reserved;
	uint8_t  flags;
	uint16_t base_hi;
} idt_gate_t;

typedef struct __packed {
	uint16_t limit;
	uint32_t base;
} idt_info_t;

void idt_init(void);
void idt_gate(uint8_t id, void(*base)(void), uint8_t flags);
