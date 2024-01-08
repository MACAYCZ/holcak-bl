ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

SECTIONS
{
	. = 0x7C00;
	__start = .;

	.text :
	{
		__text_start = .;
		*(.entry .entry.*)
		*(.text .text.*)
		. = 0x01B0;
		*(.args .args.*)
		. = 0x01FE;
		SHORT(0xAA55);
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
