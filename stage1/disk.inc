.intel_syntax noprefix
.code16

// Initializes disk
// Parameters:
//   dl    - Disk number
// References:
//   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present
//   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=08h:_Read_Drive_Parameters
disk_init:
	pushad
	mov ah, 0x41
	mov bx, 0x55AA
	xor cx, cx
	int 0x13
	mov [disk_info.extensions], cx

	mov ah, 0x08
	int 0x13
	jc disk_init.error

	inc dh
	mov [disk_info.heads], dh
	mov bl, cl
	and bl, 0x3F
	mov [disk_info.sectors], bl
	shr cx, 0x06
	inc cx
	mov [disk_info.cylinders], cx

	popad
	mov [disk_info.booting], dl
	ret

disk_init.error:
	mov al, '1'
	jmp pute

// Resets disk controller
// Parameters:
//   dl    - Disk number
disk_reset:
	push ax
	xor ah, ah
	int 0x13
	jc disk_reset.error
	pop ax
	ret

disk_reset.error:
	mov al, '2'
	jmp pute

// Reads sectors from disk
// Parameters:
//   eax   - LBA low pointing to the beginning of the data
//   ebx   - Address of buffer [offset 16] [segment 16]
//   cx    - LBA high pointing to the beginning of the data
//   si    - Number of sectors to read
//   dl    - Disk number
disk_read:
	push cx
	mov cx, [disk_info.extensions]
	test cx, 0x01
	pop cx
	jnz disk_read_0x42

	test cx, cx
	jz disk_read_0x02

disk_read.error:
	mov al, '5'
	jmp pute

// Reads sectors from disk using int 0x13, ah=0x02
// Parameters:
//   eax   - LBA pointing to the beginning of the data
//   ebx   - Address of buffer [offset 16] [segment 16]
//   si    - Number of sectors to read
//   dl    - Disk number
// References:
//   https://stanislavs.org/helppc/int_13-2.html
disk_read_0x02:
	pushad
	push es

disk_read_0x02.loop:
	push eax
	push ebx
	push dx

	// Convert LBA to CHS
	xor edx, edx
	xor ebx, ebx
	mov bl, [disk_info.sectors]
	div ebx

	inc dl
	mov cl, dl

	xor edx, edx
	mov bl, [disk_info.heads]
	div ebx

	mov dh, dl
	mov ch, al
	shl ah, 0x06
	or cl, ah

	// Initialize es:bx (Pointer to buffer)
	mov ebx, [esp+0x02]
	mov di, bx
	and di, 0x0F
	shr ebx, 0x10
	mov es, bx
	mov bx, di

	// Initialize al (Number of sectors)
	mov al, 0x80
	cmp si, 0x80
	jge disk_read_0x02.retry
	mov ax, si

	// Restore disk number
	pop di
	xor dl, dl
	or dx, di

	// Read sectors from disk
	mov di, 0x05

disk_read_0x02.retry:
	mov ah, 0x02
	push ax

	int 0x13
	jc disk_read_0x02.error

	xor ecx, ecx
	mov cl, al
	pop ax

	pop ebx
	pop eax

	sub si, cx
	test si, si
	jz disk_read_0x02.done
	add eax, ecx
	add ebx, ecx
	jmp disk_read_0x02.loop

disk_read_0x02.error:
	test di, di
	jz disk_read_0x02.fatal
	call disk_reset
	dec di
	pop ax
	jmp disk_read_0x02.retry

disk_read_0x02.fatal:
	mov al, '3'
	jmp pute

disk_read_0x02.done:
	pop es
	popad
	ret

.section .bss
	disk_info:
	disk_info.extensions: .space 0x01
	disk_info.cylinders:  .space 0x02
	disk_info.heads:      .space 0x01
	disk_info.sectors:    .space 0x01
	disk_info.booting:    .space 0x01
.section .text

// Reads sectors from disk
// Parameters:
//   eax   - LBA low pointing to the beginning of the data
//   ebx   - Address of buffer [offset 16] [segment 16]
//   cx    - LBA high pointing to the beginning of the data
//   si    - Number of sectors to read
//   dl    - Disk number
// References:
//   http://www.ctyme.com/intr/rb-0708.htm
//   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
disk_read_0x42:
	pusha

	movb [disk_packet.size],     0x10
	movb [disk_packet.reserved], 0x00
	mov  [disk_packet.sectors],  si
	mov  [disk_packet.buffer],   ebx
	mov  [disk_packet.lba.lo],   eax
	mov  [disk_packet.lba.hi],   cx

	xor cx, cx
	mov [disk_packet.lba.hi+0x02], cx

	mov si, offset disk_packet
	mov ah, 0x42
	int 0x13
	jc disk_read_0x42.error

	popa
	ret

disk_read_0x42.error:
	mov al, '4'
	jmp pute

.section .bss
	disk_packet:
	disk_packet.size:     .space  0x01
	disk_packet.reserved: .space  0x01
	disk_packet.sectors:  .space  0x02
	disk_packet.buffer:   .space  0x04
	disk_packet.lba.lo:   .space  0x04
	disk_packet.lba.hi:   .space  0x04
	disk_packet_size = . - disk_packet
.section .text
