.intel_syntax noprefix
.code16

// Detects information about memory
// References:
//   http://www.ctyme.com/intr/rb-1741.htm
mem_init:
	pusha

	// Try to detect memory
	call mem_init_0xE801
	jc mem_init.error

	// Print debug information
	mov si, offset mem_init.str_0
	call puts

	// Print lower and upper memory size
	mov cx, 0x02

	mov si, offset mem_init.str_1
	call puts
	mov si, offset mem_info.lower
	call putx
	call putnl

	mov si, offset mem_init.str_2
	call puts
	mov si, offset mem_info.upper
	call putx
	call putnl

	// Try to detect memory map
	call mem_init_0xE820
	jc mem_init.error

	// Print memory map
	xor cx, cx
	inc cx

	xor ebx, ebx
	mov eax, [mem_map.size]
	mov di, offset mem_map.data

mem_init.loop:
	mov si, offset mem_init.str_3
	call puts

	push bx
	mov si, sp
	call putx
	pop bx

	mov si, offset mem_init.str_4
	call puts

	call mem_node_print

	inc ebx
	add di, offset mem_node_size

	cmp ebx, eax
	jne mem_init.loop

	// Print debug information
	mov si, offset mem_init.str_6
	call puts

	mov cx, 0x04
	mov si, offset mem_map.bytes
	call putx
	call putnl

	mov si, offset mem_init.str_7
	call puts

	mov si, offset mem_map.size
	call putx
	call putnl

mem_init.done:
	popa
	ret

mem_init.error:
	mov si, offset mem_init.str_5
	call puts
	cli
	hlt

mem_init.str_0: .asciz "[DEBUG] Memory structure:\n\r"
mem_init.str_1: .asciz "    Lower: "
mem_init.str_2: .asciz "    Upper: "
mem_init.str_3: .asciz "    "
mem_init.str_4: .asciz ":  "
mem_init.str_5: .asciz "[ERROR] Memory initialization failed!\n\r"
mem_init.str_6: .asciz "    Bytes: "
mem_init.str_7: .asciz "    Size:  "

// Detects information about memory using int 0x15, ax=0xE801
// Returns:
//   cf    - Set if fails
// References:
//   http://www.ctyme.com/intr/rb-1739.htm
mem_init_0xE801:
	pusha
	xor cx, cx
	xor dx, dx

	// Make interrupt call
	mov ax, 0xE801
	int 0x15
	jc mem_init_0xE801.error

	// Check if output is valid
	cmp ah, 0x86
	je mem_init_0xE801.error
	cmp ah, 0x80
	je mem_init_0xE801.error

	// Move output to correct registers
	test cx, cx
	jz mem_init_0xE801.save
	mov ax, cx
	mov bx, dx

mem_init_0xE801.save:
	// Save register information
	mov [mem_info.lower], ax
	mov [mem_info.upper], bx

	popa
	clc
	ret

mem_init_0xE801.error:
	popa
	stc
	ret

// Detects memory map using int 0x15, eax=0xE820
// Returns:
//   cf    - Set if fails
// References:
//   http://www.ctyme.com/intr/rb-1741.htm
mem_init_0xE820:
	pushad
	push es

	// Initialize registers
	mov edx, 0x534D4150
	xor ebx, ebx
	mov ecx, mem_node_size
	xor esi, esi

	// Convert 32-bit address to segment:offset
	mov edi, offset mem_map.data
	mov eax, edi
	shr eax, 0x04
	mov es, ax
	and di, 0x0F

	mov eax, 0xE820
	int 0x15

	// Check if interrupt succeeded
	jc mem_init_0xE820.error
	cmp eax, edx
	jne mem_init_0xE820.error

	// Save size of memory node
	mov [mem_map.bytes], ecx
	mov edi, offset mem_map.data

mem_init_0xE820.loop:
	// Check if size was exceeded
	inc esi
	cmp esi, offset mem_map_size
	je mem_init_0xE820.warning

	// Convert 32-bit address to segment:offset
	add edi, offset mem_node_size
	push edi
	mov eax, edi
	shr eax, 0x04
	mov es, ax
	and di, 0x0F

	// Request next memory node from BIOS
	mov eax, 0xE820
	int 0x15
	pop edi

	// Check if interrupt succeeded
	jc mem_init_0xE820.error
	cmp eax, edx
	jne mem_init_0xE820.error
	mov eax, [mem_map.bytes]
	cmp eax, ecx
	jne mem_init_0xE820.error

	test ebx, ebx
	jnz mem_init_0xE820.loop

mem_init_0xE820.done:
	mov [mem_map.size], esi
	pop es
	popad
	clc
	ret

mem_init_0xE820.warning:
	push si
	mov si, offset mem_init_0xE820.str_0
	call puts
	pop si
	jmp mem_init_0xE820.done

mem_init_0xE820.error:
	mov [mem_map.size], esi
	pop es
	popad
	stc
	ret

mem_init_0xE820.str_0: .asciz "[ERROR] Memory map exceeded maximum size!\n\r"

// Prints information about memory node
// Parameters:
//   di    - Memory node address
// References:
//   https://wiki.osdev.org/Detecting_Memory_(x86)#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820
mem_node_print:
	push si
	push cx

	mov cx, 0x08
	mov si, di
	call putx

	mov si, offset mem_node_print.str_0
	call puts

	mov si, di
	add si, 0x08
	call putx

	mov si, offset mem_node_print.str_1
	call puts

	mov cx, 0x04
	mov si, di
	add si, 0x10
	call putx

	mov si, offset mem_node_print.str_2
	call puts

	pop cx
	pop si
	ret

mem_node_print.str_0: .asciz " = "
mem_node_print.str_1: .asciz " ("
mem_node_print.str_2: .asciz ")\n\r"

.struct 0x00
	mem_node:
	mem_node.base: .space 0x08
	mem_node.size: .space 0x08
	mem_node.type: .space 0x04
	mem_node.acpi: .space 0x04
	mem_node_size = . - mem_node

.section .bss
	mem_info:
	mem_info.lower: .space 0x02
	mem_info.upper: .space 0x02
	mem_map:
	mem_map.bytes:  .space 0x04
	mem_map.size:   .space 0x04
	mem_map.data:   .space mem_map_size * mem_node_size
	mem_map_size = 0xFF
.section .text
