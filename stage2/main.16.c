#include <stdnoreturn.h>
#include "console.h"

void putc(char chr);

noreturn void main16(void)
{
	putc('B');
	while (1);
}

void putc(char chr)
{
	*((char*)0x0B8000) = chr;
}
