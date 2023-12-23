.intel_syntax noprefix

.global _start
.extern main

.code16
.section .entry

_start:
	// Initialize registers
	mov ebp, 0x1000
	mov esp, ebp
	call a20_init

	// Enter protected mode
	lgdt [gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax
	jmp 0x08:entry

.include "puts.inc"
.include "a20.inc"
.include "gdt.inc"

.code32
.section .text

entry:
	// Initialize registers
	cli
	cld

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

	// Zero out bss section
	mov edi, __bss_start
	mov ecx, __bss_end
	sub ecx, edi

	xor al, al
	rep stosb

	jmp main

.section .bss
	stack: .space stack_size
	stack_size = 0x2000
