; asmsyntax=nasm
[org 0x0500]
[bits 16]

jmp main
times 0x03-($-$$) db 0x00

; BIOS Parameter Block is needed because some bioses are overwriting this section
; References:	
;   https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_4.0_EBPB
[bits 16]
bpb_oem_id:               db "HOLCAK  "
bpb_bytes_per_sector:     dw 0x0200
bpb_sectors_per_cluster:  db 0x00
bpb_reserved_sectors:     dw 0x00
bpb_number_of_fat:        db 0x00
bpb_root_entries:         dw 0x00
bpb_total_sectors:        dw 0x00
bpb_media_descriptor:     db 0x00
bpb_sectors_per_fat:      dw 0x00
bpb_sectors_per_track:    dw 0x12
bpb_number_of_heads:      dw 0x02
bpb_hidden_sectors:       dd 0x00
bpb_large_total_sectors:  dd 0x00
bpb_drive_number:         db 0x00
bpb_flags:                db 0x00
bpb_boot_signature:       db 0x00
bpb_volume_serial_number: dd 0x00
bpb_volume_label:         db "HOLCAK     "
bpb_file_system_type:     times 0x08 db 0x00

%include "puts.inc"
%include "disk.inc"

[bits 16]
main:
	; Initialize registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00

	; Relocate itself to lower address
	cld
	mov cx, 0x0200
	mov si, 0x7C00
	mov di, 0x0500
	rep movsb
	jmp 0x0000:.next

[bits 16]
.next:
	; Load stage2 into memory
	call disk_init
	mov eax, [stage2.address_lo]
	mov cx,  [stage2.address_hi]
	mov ebx, (stage2.buffer << 0x0C) | (stage2.buffer % 0x10)
	mov si,  [stage2.sectors]
	call disk_read

	; Jump to stage2
	jmp stage2.buffer

times 0x01B0-($-$$) db 0x00
stage2:
	.address_lo:  dd 0x00
	.address_hi:  dw 0x00
	.sectors:     dw 0x00
	.buffer:   equ 0x1000

times 0x01FE-($-$$) db 0x00
dw 0xAA55
