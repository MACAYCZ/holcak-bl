.intel_syntax noprefix
.code32

gdt:
gdt.null:
	.long 0x00
	.long 0x00
gdt.code:
	.word 0xFFFF
	.word 0x00
	.byte 0x00
	.byte 0x9A
	.byte 0xCF
	.byte 0x00
gdt.data:
	.word 0xFFFF
	.word 0x00
	.byte 0x00
	.byte 0x92
	.byte 0xCF
	.byte 0x00
gdt.desc:
	.word gdt.desc - gdt - 0x01
	.word gdt

code_seg = gdt.code - gdt
data_seg = gdt.data - gdt
