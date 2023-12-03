.intel_syntax noprefix
.code16

// Prints string ending with NUL
// Parameters:
//   ds:si - String address
// References:
//   https://stanislavs.org/helppc/int_10-e.html
puts:
	push ax
	push si
	mov ah, 0x0E

puts.loop:
	lodsb
	test al, al
	jz puts.done
	int 0x10
	jmp puts.loop

puts.done:
	pop si
	pop ax
	ret

// Prints ascii symbol representing error code and halts
// Parameters:
//   al    - Error code
// References:
//   https://stanislavs.org/helppc/int_10-e.html
pute:
	mov si, offset pute.str_0
	call puts
	mov ah, 0x0E
	int 0x10
	cli
	hlt

pute.str_0: .asciz "[Error] "
