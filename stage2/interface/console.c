#include <stage2/driver/port.h>
#include "console.h"

void console_clear(void)
{
	for (size_t i = 0; i < CONSOLE_ROWS * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i] = CONSOLE_DEFAULT | ' ';
	}
	console_cursor_set(0);
}

void console_scroll(size_t rows)
{
	for (size_t i = rows * CONSOLE_COLS; i < CONSOLE_ROWS * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i - rows * CONSOLE_COLS] = CONSOLE_TEXT[i];
	}
	for (size_t i = CONSOLE_ROWS * CONSOLE_COLS - rows * CONSOLE_COLS; i < rows * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i] = CONSOLE_DEFAULT | ' ';
	}
}

uint16_t console_cursor_get(void)
{
	port8_out(0x03D4, 0x0F);
	uint16_t cursor = port8_in(0x03D5);

	port8_out(0x03D4, 0x0E);
	cursor |= ((uint16_t)port8_in(0x03D5)) << 0x08;

	return cursor;
}

void console_cursor_set(uint16_t cursor)
{
	port8_out(0x03D4, 0x0F);
	port8_out(0x03D5, cursor >> 0x00);

	port8_out(0x03D4, 0x0E);
	port8_out(0x03D5, cursor >> 0x08);
}
