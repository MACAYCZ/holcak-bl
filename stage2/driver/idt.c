#include "idt.h"

static idt_gate_t idt_table[256];
static idt_info_t idt_info = {sizeof(idt_table)-1, (uint32_t)idt_table};

void idt_init(void)
{
	__asm__ volatile ("lidtl (%0)" : : "r" (&idt_info));
}

void idt_gate(uint8_t id, void(*base)(void), uint8_t flags)
{
	idt_table[id] = (idt_gate_t){(uint32_t)base >> 0x00, 0x18, 0x00, flags, (uint32_t)base >> 0x10};
}
