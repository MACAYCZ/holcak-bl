ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

__start = 0x1000;

SECTIONS
{
	. = __start;

	.text :
	{
		__text_start = .;
		build/stage2/entry.asm.o(.text .text.*)
		*(.text .text.*)
	}

	.data   : {__data_start   = .; *(.data .data.*) *(.bss .bss.*)}
	.rodata : {__rodata_start = .; *(.rodata .rodata.*)           }

	__end = .;
}
