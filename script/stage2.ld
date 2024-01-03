ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

SECTIONS
{
	. = 0x8000;
	__start = .;

	.text :
	{
		__text_start = .;
		*(.entry .entry.*)
		*(.text .text.*)
		__text_end = .;
	}

	.rodata :
	{
		__rodata_start = .;
		*(.rodata .rodata.*)
		__rodata_end = .;
	}

	.data :
	{
		__data_start = .;
		*(.data .data.*)
		__data_end = .;
	}

	.bss :
	{
		__bss_start = .;
		*(.bss .bss.*)
		__bss_end = .;
	}

	__end = .;
}
