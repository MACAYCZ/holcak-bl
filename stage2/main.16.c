#include <stdnoreturn.h>
#include "console.h"

noreturn void main16(void)
{
	*((char*)0x0B8000) = 'A';
	while (1);
}
