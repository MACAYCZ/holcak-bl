#include <stddef.h>

void puts(const char *str)
{
	for (size_t i = 0; str[i]; i++)
	{
		*((char*)0xB8000 + i * 0x02) = str[i];
	}
}

_Noreturn void main(void)
{
	puts("Hello, World!");
	while (1);
}
