#include <stage2/driver/x86_16.h>
#include <stage2/driver/port.h>
#include "console.h"

uint16_t console_cursor = 0;
uint8_t console_color = CONSOLE_COLOR_DEFAULT;

void console_init(void)
{
	x86_16_regs_t input = {
		.eax = 0x03,
	};
	x86_16_regs_t output;
	x86_16_int(0x10, &input, &output);

	console_cursor_show(CONSOLE_CURSOR_UNDERLINE);
	console_clear();
	console_flush();
}

void console_print(char character)
{
	if (character == '\n') {
		if (console_cursor >= (CONSOLE_ROWS - 1) * CONSOLE_COLS) {
			console_cursor = (CONSOLE_ROWS - 1) * CONSOLE_COLS;
			console_scroll(1);
		} else {
			console_cursor = (console_cursor + CONSOLE_COLS - 1) / CONSOLE_COLS * CONSOLE_COLS;
		}
	} else {
		CONSOLE_TEXT[console_cursor] = (console_color << 0x08) | character;
		console_cursor += 1;
	}
}

void console_flush(void)
{
	port8_out(0x03D4, 0x0F);
	port8_out(0x03D5, console_cursor >> 0x00);

	port8_out(0x03D4, 0x0E);
	port8_out(0x03D5, console_cursor >> 0x08);
}

void console_clear(void)
{
	for (size_t i = 0; i < CONSOLE_ROWS * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i] = (console_color << 0x08) | ' ';
	}
	console_cursor = 0;
}

void console_scroll(uint8_t rows)
{
	for (size_t i = rows * CONSOLE_COLS; i < CONSOLE_ROWS * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i - rows * CONSOLE_COLS] = CONSOLE_TEXT[i];
	}
	for (size_t i = CONSOLE_ROWS * CONSOLE_COLS - rows * CONSOLE_COLS; i < rows * CONSOLE_COLS; i++) {
		CONSOLE_TEXT[i] = (console_color << 0x08 | ' ');
	}
}

void console_cursor_show(uint8_t shape)
{
	port8_out(0x03D4, 0x0A);
	port8_out(0x03D5, (port8_in(0x03D5) & 0xC0) | (shape >> 0x04));

	port8_out(0x03D4, 0x0B);
	port8_out(0x03D5, (port8_in(0x03D5) & 0xE0) | (shape & 0xFF));
}

void console_cursor_hide(void)
{
	port8_out(0x03D4, 0x0A);
	port8_out(0x03D5, 0x20);
}
