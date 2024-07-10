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

int get_screen_offset(int row, int col);
int get_cursor();
void set_cursor(int offset);
int handle_scrolling(int cursor_offset);

void port_byte_out(unsigned short port, unsigned char data);
unsigned char port_byte_in(unsigned short port);

void print_char(char ch, int row, int col, char attribute_byte);
void print_at(const char* message, int row, int col);
void print(const char* message);

void clear_screen();

#endif