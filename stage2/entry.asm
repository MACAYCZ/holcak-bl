.intel_syntax noprefix
.code16

.section .text
.extern main16

.global _start
_start:
	mov sp, 0x1000
	mov bp, sp
	jmp main16
