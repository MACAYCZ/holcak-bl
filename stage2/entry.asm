.intel_syntax noprefix
.code16

.extern main16

.global _start
_start:
	mov sp, 0x1000
	mov bp, sp

	xor ax, ax
	mov es, ax
	mov ds, ax
	mov ss, ax

	jmp main16
