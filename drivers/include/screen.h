#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print_char(char ch, int row, int col, char attribute_byte);
void print_at(char* message, int row, int col);
void print(char* message);

void clear_screen();

#endif