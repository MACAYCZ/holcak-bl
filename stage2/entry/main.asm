.intel_syntax noprefix

.global _start
.extern main

.code16
.section .entry

_start:
	// Initialize registers
	mov ebp, 0x8000
	mov esp, ebp

	// Set video mode
	xor ah, ah
	mov al, 0x03
	int 0x10
	cmp al, 0x30
	jne $

	// Enter protected mode
	lgdt [gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax
	jmp 0x18:entry

.include "gdt.inc"
.code32

entry:
	// Initialize registers
	cli
	cld

	xor ax, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ax, 0x20
	mov ds, ax
	mov ss, ax

	// Zero out bss section
	mov edi, __bss_start
	mov ecx, __bss_end
	sub ecx, edi
	xor al, al
	rep stosb

	jmp main
