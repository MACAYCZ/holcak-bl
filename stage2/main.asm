[bits 16]

[global entry]
entry:
	mov al, 'A'
	mov ah, 0x0E
	int 0x10

	cli
	hlt
