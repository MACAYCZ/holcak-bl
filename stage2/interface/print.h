#pragma once
#include <stage2/interface/console.h>
#include <stdarg.h>

void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list args);
