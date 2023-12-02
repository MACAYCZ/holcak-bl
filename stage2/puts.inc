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

// Prints hexadecimal digit (value greater than 0x0F results in UB)
// Parameters:
//   al    - Value
// References:
//   https://stanislavs.org/helppc/int_10-e.html
putxd:
	push ax
	mov ah, 0x0E

	cmp al, 0x0A
	jl putxd.dump
	add al, 0x07

putxd.dump:
	add al, 0x30
	int 0x10

	pop ax
	ret	

// Prints little-endian integer in hexadecimal format
// Parameters:
//   ds:si - Integer address
//   cx    - Size
// References:
//   https://stanislavs.org/helppc/int_10-e.html
putx:
	test cx, cx
	jnz putx.next
	ret

putx.next:
	push si
	mov si, offset putx.prefix
	call puts
	pop si

	pusha
	add si, cx
	dec si

putx.loop:
	std
	lodsb
	cld

	mov bl, al
	shr al, 0x04
	call putxd

	mov al, bl
	and al, 0x0F
	call putxd

	dec cx
	jnz putx.loop

putx.done:
	popa
	ret

putx.prefix:
	.asciz "0x"

// Moves cursor to new line
// References:
//   https://stanislavs.org/helppc/int_10-e.html
putnl:
	push si
	mov si, offset putnl.data
	call puts
	pop si
	ret

putnl.data:
	.asciz "\n\r"
