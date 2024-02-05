#pragma once
#include <stdarg.h>

void putc(char chr);
void puts(const char *str);
void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list args);
