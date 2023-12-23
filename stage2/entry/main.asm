.intel_syntax noprefix

.global _start
.extern main

.code16
.section .entry

_start:
	// Initialize registers
	mov ebp, 0x1000
	mov esp, ebp

	// Set video mode
	xor ah, ah
	mov al, 0x03
	int 0x10
	cmp al, 0x30
	je _start.next

	// Print error message
	mov si, offset _start.str_0
	call puts
	cli
	hlt

_start.next:
	// Enable line A20
	call a20_init

	// Enter protected mode
	lgdt [gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax
	jmp 0x08:entry

_start.str_0:
	.asciz "Initializing video mode failed!\n\r"

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
