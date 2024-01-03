ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

MEMORY
{
	ROM(RX)  : ORIGIN = 0x7C00, LENGTH = 0x0200
	RAM(RWX) : ORIGIN = 0x7E00, LENGTH = 0x0781FF
}

SECTIONS
{
	. = 0x7C00;
	__start = .;

	.text :
	{
		__text_start = .;
		*(.text .text.*)
		__text_end = .;
	}

	.bss :
	{
		__bss_start = .;
		*(.bss .bss.*)
		__bss_end = .;
	}

	__end = .;
}
