ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

MEMORY
{
	RAM(RWX) : ORIGIN = 0x0500, LENGTH = 0x7700
	ROM(RX)  : ORIGIN = 0x7C00, LENGTH = 0x0200
}

SECTIONS
{
	. = 0x0500;
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
