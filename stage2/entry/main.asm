.intel_syntax noprefix
.code16

.section .text
.include "puts.inc"
.include "a20.inc"
.include "mem.inc"

.section .entry
.global _start

_start:
	// Initialize registers
	mov sp, 0x1000
	mov bp, sp

	call mem_init
	call a20_init

	// Enter protected mode
	lgdt [gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax
	jmp 0x08:entry

.section .text
.code32

.extern main
entry:
	// Initialize registers
	cli
	xor ax, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ax, 0x10
	mov ds, ax
	mov ss, ax

	// Initialize stack
	mov ebp, offset stack + offset stack_size
	mov esp, ebp

	jmp main

.include "gdt.inc"

.section .bss
	stack: .space stack_size
	stack_size = 0x1000
