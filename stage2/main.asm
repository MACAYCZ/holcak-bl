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

	cli
	hlt
