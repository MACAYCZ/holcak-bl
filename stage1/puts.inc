; asmsyntax=nasm
%define endl 0x0D, 0x0A

; Prints string ending with NUL
; Parameters:
;   ds:si - String address
; References:
;   https://stanislavs.org/helppc/int_10-e.html
[bits 16]
puts:
	push ax
	push si
	mov ah, 0x0E

.loop:
	lodsb
	test al, al
	jz .done
	int 0x10
	jmp .loop

.done:
	pop si
	pop ax
	ret

; Prints ascii symbol representing error code and halts
; Parameters:
;   al    - Error code
; References:
;   https://stanislavs.org/helppc/int_10-e.html
[bits 16]
pute:
	mov si, .str0
	call puts
	mov ah, 0x0E
	int 0x10
	cli
	hlt

.str0:
	db "Error: ", 0x00
