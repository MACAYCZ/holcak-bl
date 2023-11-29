.intel_syntax noprefix
.code16

.section .entry
.extern main16

.global _start
_start:
	mov sp, 0x1000
	mov bp, sp
	jmp main16
