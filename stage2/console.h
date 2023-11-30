#pragma once
#include <stdarg.h>

void printf(const char *fmt)
{
	*((char*)0x0B8000) = *fmt;
}

/*
void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list args);

void printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void vprintf(const char *fmt, va_list args)
{
	(void)args;
	*((char*)0xB8000) = fmt[0];
}
*/
