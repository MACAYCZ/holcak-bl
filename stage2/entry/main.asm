// vim: ft=asm
.intel_syntax noprefix
.code16

.global _start
.extern main

.section .entry

_start:
	// Initialize registers
	mov ebp, 0x8000
	mov esp, ebp

	// Enter protected mode
	lgdt [gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp 0x18:_start.next

.include "gdt.inc"
.code32

_start.next:
	// Initialize registers
	cli
	cld

	mov ax, 0x20
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	// Zero out bss section
	mov edi, __bss_start
	mov ecx, __bss_end
	sub ecx, edi
	xor al, al
	rep stosb

	jmp main
