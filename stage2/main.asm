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

	cli
	hlt
