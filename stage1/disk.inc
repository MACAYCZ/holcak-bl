; asmsyntax=nasm

; Initializes disk
; Parameters:
;   dl    - Disk number
; References:
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=08h:_Read_Drive_Parameters
[bits 16]
disk_init:
	pushad
	mov ah, 0x41
	mov bx, 0x55AA
	xor cx, cx
	int 0x13
	mov [disk_info.extensions], cx

.continue:
	mov ah, 0x08
	int 0x13
	jc .error

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

.error:
	mov al, '1'
	jmp pute

; Resets disk controller
; Parameters:
;   dl    - Disk number
[bits 16]
disk_reset:
	push ax
	xor ah, ah
	int 0x13
	jc .error
	pop ax
	ret

.error:
	mov al, '2'
	jmp pute

; Reads sectors from disk
; Parameters:
;   eax   - LBA low pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   cx    - LBA high pointing to the beginning of the data
;   si    - Number of sectors to read
;   dl    - Disk number
[bits 16]
disk_read:
	push cx
	mov cx, [disk_info.extensions]
	test cx, 0x01
	pop cx
	jnz disk_read_0x42

	test cx, cx
	jz disk_read_0x02

.error:
	mov al, '5'
	jmp pute

; Reads sectors from disk using int 0x13, ah=0x02
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   https://stanislavs.org/helppc/int_13-2.html
[bits 16]
disk_read_0x02:
	pushad
	push es

.loop:
	push eax
	push ebx
	push dx

	; Convert LBA to CHS
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

	; Initialize es:bx (Pointer to buffer)
	mov ebx, [esp+0x02]
	mov di, bx
	and di, 0x0F
	shr ebx, 0x10
	mov es, bx
	mov bx, di

	; Initialize al (Number of sectors)
	mov al, 0x80
	cmp si, 0x80
	jge .retry
	mov ax, si

	; Restore disk number
	pop di
	xor dl, dl
	or dx, di

	; Read sectors from disk
	mov di, 0x05

.retry:
	mov ah, 0x02
	push ax

	int 0x13
	jc .error

	xor ecx, ecx
	mov cl, al
	pop ax

	pop ebx
	pop eax

	sub si, cx
	test si, si
	jz .done
	add eax, ecx
	add ebx, ecx
	jmp .loop

.error:
	test di, di
	jz .fatal
	call disk_reset
	dec di
	pop ax
	jmp .retry

.fatal:
	mov al, '3'
	jmp pute

.done:
	pop es
	popad
	ret

section .bss
disk_info:
	.extensions: resb 0x01
	.cylinders:  resw 0x01
	.heads:      resb 0x01
	.sectors:    resb 0x01
	.booting:    resb 0x01
section .text

; Reads sectors from disk
; Parameters:
;   eax   - LBA low pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   cx    - LBA high pointing to the beginning of the data
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
[bits 16]
disk_read_0x42:
	pusha

	mov byte [disk_packet.size], 0x10
	mov byte [disk_packet.reserved], 0x00
	mov word [disk_packet.sectors], si
	mov dword [disk_packet.buffer], ebx
	mov dword [disk_packet.lba_lo], eax
	mov word [disk_packet.lba_hi], cx

	xor cx, cx
	mov [disk_packet.lba_hi+0x02], cx

	mov si, disk_packet
	mov ah, 0x42
	int 0x13
	jc .error

	popa
	ret

.error:
	mov al, '4'
	jmp pute

section .bss
disk_packet:
	.size:     resb 0x01
	.reserved: resb 0x01
	.sectors:  resw 0x01
	.buffer:   resd 0x01
	.lba_lo:   resd 0x01
	.lba_hi:   resd 0x01
section .text
