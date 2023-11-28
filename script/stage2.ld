ENTRY(_start)

__start = 0x1000;

SECTIONS
{
	. = __start;

	.entry  : {__entry_start  = .; *(.entry) }
	.text   : {__text_start   = .; *(.text)  }
	.data   : {__data_start   = .; *(.data)  }
	.rodata : {__rodata_start = .; *(.rodata)}
	.bss    : {__bss_start    = .; *(.bss)   }

	__end = .;
}
