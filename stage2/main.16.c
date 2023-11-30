#include <stdnoreturn.h>
#include "console.h"

noreturn void main16(void)
{
	char *str = "Hello, World!";

	printf(str);
	while (1);
}
