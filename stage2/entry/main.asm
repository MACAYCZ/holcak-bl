// vim: ft=asm
.intel_syntax noprefix
.code16

.extern main
.section .entry

.global _start
_start:
	// Initialize registers
	mov ebp, 0x7C00
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
	mov edi, offset __bss_start
	mov ecx, offset __bss_end
	sub ecx, edi
	xor al, al
	rep stosb

	// Call main with arguments
	xor dh, dh
	push dx
	call main
