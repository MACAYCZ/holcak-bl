ENTRY(_start)

__start = 0x0500;

SECTIONS
{
	. = __start;

	.text   : {__text_start   = .; *(.text)  }
	.data   : {__data_start   = .; *(.data)  }
	.rodata : {__rodata_start = .; *(.rodata)}
	.bss    : {__bss_start    = .; *(.bss)   }

	__end = .;
}
