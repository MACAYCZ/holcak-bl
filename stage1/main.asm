.intel_syntax noprefix
.code16

.section .text
.global _start

_start:
	jmp main

// BIOS Parameter Block is required because some bioses are overwriting this section.
// References:	
//   https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_4.0_EBPB
.zero 0x03 - (. - _start)
	bpb.oem_id:               .ascii "HOLCAK  "
	bpb.bytes_per_sector:     .word 0x0200
	bpb.sectors_per_cluster:  .byte 0x00
	bpb.reserved_sectors:     .word 0x00
	bpb.number_of_fat:        .byte 0x00
	bpb.root_entries:         .word 0x00
	bpb.total_sectors:        .word 0x00
	bpb.media_descriptor:     .byte 0x00
	bpb.sectors_per_fat:      .word 0x00

	bpb.sectors_per_track:    .word 0x12
	bpb.number_of_heads:      .word 0x02
	bpb.hidden_sectors:       .long 0x00
	bpb.large_total_sectors:  .long 0x00

	bpb.drive_number:         .byte 0x00
	bpb.flags:                .byte 0x00
	bpb.boot_signature:       .byte 0x00
	bpb.volume_serial_number: .long 0x00
	bpb.volume_label:         .ascii "HOLCAK     "
	bpb.file_system_type:     .zero 0x08

.include "puts.inc"
.include "disk.inc"

main:
	// Initialize registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x1000
	mov bp, sp

	// Load stage2 into memory
	call disk_init
	mov eax, [stage2.address.lo]
	mov cx,  [stage2.address.hi]
	mov ebx, (stage2.buffer << 0x0C) | (stage2.buffer % 0x10)
	mov si,  [stage2.sectors]
	call disk_read

	jmp stage2.buffer

.zero 0x01B0 - (. - _start)
	stage2.address.lo: .long 0x00
	stage2.address.hi: .word 0x00
	stage2.sectors:    .word 0x00
	stage2.buffer        = 0x8000

.zero 0x01FE - (. - _start)
.word 0xAA55
