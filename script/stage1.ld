ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

__start = 0x0500;

SECTIONS
{
	. = __start;

	.text : {__text_start = .; *(.text)}
	.bss  : {__bss_start  = .; *(.bss) }

	__end = .;
}
