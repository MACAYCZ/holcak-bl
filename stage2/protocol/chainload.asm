.intel_syntax noprefix
.code32

.global chainload_jump

// Initializes registers and jumps to 0x7C00
// Parameters:
//   0x01  - Booting device
chainload_jump:
	// Save booting device
	mov dl, [esp+0x04]

	// Jump to 16-bit mode
	lidt [chainload_jump.ivt]
	jmp 0x08:chainload_jump.real

chainload_jump.real:
	.code16

	mov eax, cr0
	and al, 0xFE
	mov cr0, eax
	jmp 0x00:chainload_jump.zero

chainload_jump.zero:
	// Initialize registers
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	and edx, 0xFF
	xor esi, esi
	xor edi, edi

	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	// Clear flags
	pushd 0x00
	popfd

	// Initialize stack
	mov esp, 0x7C00
	mov ebp, esp

	jmp 0x00:0x7C00

chainload_jump.ivt:
	.word 0x03FF
	.long 0x00
