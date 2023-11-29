ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")

__start = 0x1000;

SECTIONS
{
	. = __start;

	.text   : {__text_start   = .; *(.text)  }
	.data   : {__data_start   = .; *(.data)  }
	.rodata : {__rodata_start = .; *(.rodata)}
	.bss    : {__bss_start    = .; *(.bss)   }

	__end = .;
}
