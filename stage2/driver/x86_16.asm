// vim: ft=asm
.intel_syntax noprefix
.code32

.section .text
.global x86_16_int

// Calls BIOS interrupt from protected mode
// References:
//   https://github.com/limine-bootloader/limine/blob/1ae036c3feb3717d1f6171e43e389eb8214bd963/common/lib/real.s2.asm_bios_ia32
x86_16_int:
	// Initialize interrupt
	mov al, [esp+0x04]
	mov [x86_16_int.int], al

	// Save address of registers
	mov eax, [esp+0x08]
	mov [x86_16_int.regs], eax

	// Save descriptors in case BIOS overwrites them
	sgdt [x86_16_int.gdt]
	sidt [x86_16_int.idt]
	lidt [x86_16_int.ivt]

	// Save non-scratch GPRs
	push ebx
	push esi
	push edi
	push ebp

	// Jump to 16-bit mode
	jmp 0x08:x86_16_int.real

x86_16_int.real:
	.code16

	mov eax, cr0
	and al, 0xFE
	mov cr0, eax
	jmp 0x00:x86_16_int.zero

x86_16_int.zero:
	xor ax, ax
	mov ss, ax

	// Load input registers
	mov ss:[x86_16_int.esp], esp
	mov esp, ss:[x86_16_int.regs]
	pop gs
	pop fs
	pop es
	pop ds
	popfd
	pop ebp
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
//	mov esp, ss:[x86_16_int.esp]
	sti

	// Make interrupt call
	.byte 0xCD
x86_16_int.int:
	.byte 0x00

	// Save output registers
	cli
//	mov ss:[x86_16_int.esp], esp
//	mov esp, ss:[x86_16_int.output]
//	lea esp, [esp+0x28]
	push eax
	push ebx
	push ecx
	push edx
	push esi
	push edi
	push ebp
	pushfd
	push ds
	push es
	push fs
	push gs
	mov esp, ss:[x86_16_int.esp]

	// Restore descriptors
	lgdt ss:[x86_16_int.gdt]
	lidt ss:[x86_16_int.idt]

	// Jump to 32-bit mode
	mov eax, cr0
	or al, 0x01
	mov cr0, eax

	jmp 0x18:x86_16_int.done

x86_16_int.done:
	.code32

	// Restore segment registers
	mov ax, 0x20
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	// Restore non-scratch GPRs
	pop ebp
	pop edi
	pop esi
	pop ebx

	ret

x86_16_int.ivt:
	.word 0x03FF
	.long 0x00

x86_16_int.gdt:
	.quad 0x00

x86_16_int.idt:
	.quad 0x00

x86_16_int.regs:
	.long 0x00

x86_16_int.esp:
	.long 0x00
