.intel_syntax noprefix
.code16

.section .text
.include "puts.inc"

.section .entry
.global _start

_start:
	// Initialize registers
	mov sp, 0x1000
	mov bp, sp

	mov cx, 0x02
	mov si, offset buffer
	call putx

	cli
	hlt

.section .rodata
	buffer: .byte 0xAB, 0xCD
