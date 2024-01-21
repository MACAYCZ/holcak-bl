#pragma once
#include <stdint.h>
#include <stddef.h>

#define CONSOLE_TEXT ((uint16_t*)0x0B8000)
#define CONSOLE_ROWS 25
#define CONSOLE_COLS 80

#define CONSOLE_FOREGROUND(Color) ((Color) << 0x00)
#define CONSOLE_BACKGROUND(Color) ((Color) << 0x04)
#define CONSOLE_COLOR(Foreground, Background) (CONSOLE_FOREGROUND(Foreground) | CONSOLE_BACKGROUND(Background))
#define CONSOLE_COLOR_DEFAULT CONSOLE_COLOR(CONSOLE_LIGHT_GRAY, CONSOLE_BLACK)

#define CONSOLE_CURSOR(Row, Col) ((Col) + (Row) * CONSOLE_COLS)
#define CONSOLE_CURSOR_BLOCK 0x0F
#define CONSOLE_CURSOR_UNDERLINE 0xFF

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

extern uint16_t console_cursor;
extern uint8_t console_color;

void console_init(void);
void console_print(char character);
void console_flush(void);
void console_clear(void);
void console_scroll(uint8_t rows);

void console_cursor_show(uint8_t shape);
void console_cursor_hide(void);
