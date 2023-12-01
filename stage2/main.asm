[bits 16]

[global entry16]
entry16:
	mov al, 'A'
	mov ah, 0x0E
	int 0x10

	cli
	hlt
