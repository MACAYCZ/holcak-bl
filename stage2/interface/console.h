#pragma once
#include <stdint.h>
#include <stddef.h>

#define CONSOLE_TEXT ((uint16_t*)0x0B8000)
#define CONSOLE_ROWS 25
#define CONSOLE_COLS 80

#define CONSOLE_DEFAULT CONSOLE_ATTRIBUTES(CONSOLE_LIGHT_GRAY, CONSOLE_BLACK)

#define CONSOLE_CURSOR(Row, Col) ((Col) + (Row) * CONSOLE_COLS)
#define CONSOLE_FOREGROUND(Color) ((Color) << 0x00)
#define CONSOLE_BACKGROUND(Color) ((Color) << 0x04)
#define CONSOLE_ATTRIBUTES(Foreground, Background) (CONSOLE_FOREGROUND(Foreground) | CONSOLE_BACKGROUND(Background))

typedef enum {
	CONSOLE_BLACK,
	CONSOLE_BLUE,
	CONSOLE_GREEN,
	CONSOLE_CYAN,
	CONSOLE_RED,
	CONSOLE_MAGENTA,
	CONSOLE_BROWN,
	CONSOLE_LIGHT_GRAY,
	CONSOLE_DARK_GRAY,
	CONSOLE_LIGHT_BLUE,
	CONSOLE_LIGHT_GREEN,
	CONSOLE_LIGHT_CYAN,
	CONSOLE_LIGHT_RED,
	CONSOLE_LIGHT_MAGENTA,
	CONSOLE_YELLOW,
	CONSOLE_WHITE,
} console_color_t;

void console_clear(void);
void console_scroll(size_t rows);
uint16_t console_cursor_get(void);
void console_cursor_set(uint16_t cursor);
