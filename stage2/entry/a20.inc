.intel_syntax noprefix
.code16

// Enables A20 line
// References:
//   https://wiki.osdev.org/A20_Line#Enabling
a20_init:
	push ax
	call a20_test
	jc a20_init.done

a20_init.try_0:
	// Try int 0x15, ax=0x2403
	mov ax, 0x2403
	int 0x15
	call a20_test
	jc a20_init.done

a20_init.try_1:
	// Try 'Fast A20 Gate'
	xor al, al
	mov ah, 0x20
	in al, 0x92
	or al, 0x02
	out 0x92, al
	call a20_test
	jc a20_init.done
	test ah, ah
	dec ah
	jnz a20_init.try_1

a20_init.try_2:
	// Try 'Access of 0xEE'
	in al, 0xEE
	call a20_test
	jc a20_init.done

a20_init.error:
	// Print error message
	mov si, offset a20_init.str_1
	call puts

	cli
	hlt

a20_init.done:
	// Print debug message
	push si
	mov si, offset a20_init.str_0
	call puts
	pop si

	pop ax
	ret

a20_init.str_0: .asciz "[DEBUG] A20 enabled!\n\r"
a20_init.str_1: .asciz "[ERROR] A20 cannot be enabled!\n\r"

// Tests if A20 line is enabled
// Returns:
//   cf    - Set if A20 line is enabled
// References:
//   https://wiki.osdev.org/A20_Line#Testing_the_A20_line
a20_test:
	push ax
	push bx
	push ds
	push es
	clc

	// Initialize registers
	xor ax, ax
	mov ds, ax
	mov si, 0x0500

	not ax
	mov es, ax
	mov di, 0x0510

	// Save old values
	mov bl, ds:[si]
	mov bh, es:[di]

	// Test if enabled
	mov ah, 0x01
	movb ds:[si], 0x00
	movb es:[di], 0x01
	mov al, ds:[si]
	cmp al, es:[di]
	je a20_test.done
	stc

a20_test.done:
	// Restore values
	mov ds:[si], bl
	mov es:[di], bh

	pop es
	pop ds
	pop bx
	pop ax
	ret
