; Detects free regions of memory
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init:
	pushad
	push es

	; Initialize registers
	mov eax, 0xE820
	xor ebx, ebx
	mov ecx, mem_node_size
	mov edx, 0x534D4150

	; Fill memory info with zeros
	mov [mem_info.length],   bx
	mov [mem_info.block],    bx
	mov [mem_info.total_lo], ebx
	mov [mem_info.total_hi], ebx

	; Initialize es:di (Pointer to buffer)
	mov edi, mem_info.data
	shr edi, 0x04
	mov es, di
	mov edi, mem_info.data
	and di, 0x0F

	; Check if int 0x15, ah=0xE820 is supported
	int 0x15
	cmp eax, edx
	jne .error

	; Save size of `mem_node`
	mov [mem_info.block], cx
	pop es

	call mem_init_0xE820

.dump:
	; Print debug information
	mov si, .msg1
	call puts

	xor ecx, ecx
	mov ax, [mem_info.length]

.loop:
	cmp ax, cx
	je .done

	mov si, .msg4
	call puts
	mov bl, cl
	call putx8
	mov si, .msg5
	call puts

	push eax
	mov eax, ecx
	mov edx, mem_node_size
	mul edx

	mov edi, eax
	add edi, mem_info.data
	call mem_node_print

	; Add value to total
	mov eax, [mem_info.total_lo]
	add eax, [edi+0x08]
	mov [mem_info.total_lo], eax
	jnc .skip

	; Add carry to total
	mov eax, [mem_info.total_hi]
	inc eax
	mov [mem_info.total_hi], eax

.skip:
	pop eax

	inc cx
	jmp .loop

.done:
	mov si, .msg2
	call puts
	xor ebx, ebx
	mov bx, [mem_info.length]
	call putx32
	call putnl

	mov si, .msg3
	call puts
	xor ebx, ebx
	mov bx, [mem_info.block]
	call putx32
	call putnl

	mov si, .msg6
	call puts
	mov ebx, [mem_info.total_hi]
	call putx32
	mov ebx, [mem_info.total_lo]
	call putx32
	call putnl
	
	popad
	ret

.error:
	mov si, .msg0
	call puts
	cli
	hlt

.msg0: db "Error: MEM initialization failed!", 0x00
.msg1: db "MEM detected!", endl, 0x00
.msg2: db "    Length:    0x", 0x00
.msg3: db "    Block:     0x", 0x00
.msg4: db "    Node 0x", 0x00
.msg5: db ": 0x", 0x00
.msg6: db "    Total:     0x", 0x00

; Detects free regions of memory using int 0x15, ax=0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init_0xE820:
	pushad

	; Initialize registers
	xor ebx, ebx
	mov edx, 0x534D4150
	xor ecx, ecx
	mov cx, [mem_info.block]
	mov edi, mem_info.data
	xor esi, esi

.loop:
	; Test if we exceeded maximum length
	cmp esi, mem_data_size
	jge .warning

	; Initialize es:di (Pointer to buffer)
	push edi
	shr edi, 0x04
	mov es, di
	mov edi, [esp+0x00]
	and di, 0x0F

	mov eax, 0xE820
	int 0x15
	jc .error

	; Test if size of block is valid
	xor eax, eax
	mov ax, [mem_info.block]
	cmp eax, ecx
	jne .error

	; Add offset to buffer and increment length
	pop edi
	add edi, mem_node_size
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [mem_info.length], si
	popad
	ret

.warning:
	push si
	mov si, .msg1
	call puts
	pop si
	jmp .done

.error:
	mov si, .msg0
	call puts
	cli
	hlt

.msg0: db "Error: MEM initialization failed!", 0x00
.msg1: db "MEM info length exceeded maximum!", endl, 0x00

; Prints information about memory node
; Parameters:
;   edi   - Memory node address
mem_node_print:
	pushad

	mov si, .msg0
	call puts

	mov ebx, [edi+0x04]
	call putx32
	mov ebx, [edi+0x00]
	call putx32

	mov si, .msg1
	call puts

	mov ebx, [edi+0x0C]
	call putx32
	mov ebx, [edi+0x08]
	call putx32

	mov si, .msg2
	call puts

	mov ebx, [edi+0x10]
	call putx32

	mov si, .msg3
	call puts

	popad
	ret

.msg0: db "", 0x00
.msg1: db " = 0x", 0x00
.msg2: db " (0x", 0x00
.msg3: db ")", endl, 0x00

[global mem_info]
mem_data_size: equ 0xFF
section .bss
struc mem_node
	.base:     resq 0x01
	.size:     resq 0x01
	.type:     resd 0x01
	.acpi:     resd 0x01
endstruc
mem_info:
	.length:   resw 0x01
	.block:    resw 0x01
	.total_lo: resd 0x01
	.total_hi: resd 0x01
	.data:     resb mem_data_size*mem_node_size
section .text
