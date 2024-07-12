#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

#define CHARACTER_SIZE 2    // 2 bytes -> character + attribute

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

extern int get_screen_offset(int row, int col);
extern int get_cursor();
extern void set_cursor(int offset);
extern int handle_scrolling(int cursor_offset);

extern void print_char(char ch, int row, int col, char attribute_byte);
extern void print_at(const char* message, int row, int col);
extern void print(const char* message);

extern void clear_screen();
extern void delete_last_char();

#endif