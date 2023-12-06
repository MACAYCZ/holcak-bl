// NOTE: https://github.com/limine-bootloader/limine/blob/1ae036c3feb3717d1f6171e43e389eb8214bd963/common/lib/real.s2.asm_bios_ia32

.intel_syntax noprefix
.code32

.section .text

.global x86_16_int

x86_16_int:
	.code32
	jmp 0x18:x86_16_int.real

x86_16_int.real:
	.code16

	mov eax, cr0
	and al, 0xFE
	mov cr0, eax

	jmp 0x00:x86_16_int.zero

x86_16_int.zero:
	xor ax, ax
	mov ds, ax
	mov ss, ax

	sti

	mov al, 'A'
	mov ah, 0x0E
	int 0x10

	cli

	mov eax, cr0
	or al, 0x01
	mov cr0, eax

	jmp 0x08:x86_16_int.done

x86_16_int.done:
	.code32

	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	ret

/*
// Calls BIOS interrupt from protected mode
// References:
//   https://wiki.osdev.org/Real_Mode#Switching_from_Protected_Mode_to_Real_Mode
x86_16_int:
	cli
	movb al, [esp+0x04]
	movb [x86_16_int.int], al

	// Save output registers
	movd eax, [esp+0x08]
	movd [x86_16_int.output], eax

	// Save input registers
	movd eax, [esp+0x0C]
	movd [x86_16_int.input], eax

	// Save GDT and IDT in case BIOS overwrites it
	sgdt [x86_16_int.gdt]
	sidt [x86_16_int.idt]

	// Load BIOS IVT (real mode IDT)
	lidt [x86_16_int.ivt]

	// Save non-scratch GPRs
	push ebx
	push esi
	push edi
	push ebp

	// Jump to real mode
	jmp 0x18:x86_16_int.real

x86_16_int.real:
	.code16

	// Disable protected mode bit
	mov eax, cr0
	and al, 0xFE
	mov cr0, eax

	// Jump to real mode
	jmp 0x00:x86_16_int.zero

x86_16_int.zero:
	.code16

	// Initialize registers
	xor ax, ax
	mov ss, ax

	// Load input registers
	movd ss:[x86_16_int.esp], esp
	movd esp, ss:[x86_16_int.input]
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
	movd esp, ss:[x86_16_int.esp]

	// Call BIOS interrupt
	sti
	.byte 0xCD

x86_16_int.int:
	.byte 0x00
	cli

	// Save output registers
	movd ss:[x86_16_int.esp], esp
	movd esp, ss:[x86_16_int.output]
	lea esp, [esp+0x28]
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
	movd esp, ss:[x86_16_int.esp]

	// Restore GDT and IDT
	.code32
	lgdt ss:[x86_16_int.gdt]
	lidt ss:[x86_16_int.idt]
	.code16

	// Enable protected mode bit
	mov eax, cr0
	or al, 0x01
	mov cr0, eax

	// Jump to protected mode
	jmp 0x08:x86_16_int.done

x86_16_int.done:
	.code32

	// Restore non-scratch GPRs
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

x86_16_int.input:
	.long 0x00

x86_16_int.output:
	.long 0x00

x86_16_int.esp:
	.long 0x00

x86_16_int.gdt:
	.quad 0x00

x86_16_int.idt:
	.quad 0x00

x86_16_int.ivt:
	.word 0x03FF
	.long 0x00
*/
