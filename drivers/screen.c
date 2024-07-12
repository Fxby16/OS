#include <memory.h>

#include "include/screen.h"
#include "../kernel/include/ports_io.h"

void print_char(char ch, int row, int col, char attribute_byte)
{
    unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
    
    if(!attribute_byte){    // if attribute byte is zero, assume white on black
        attribute_byte = WHITE_ON_BLACK;
    }

    int offset;
    if(row >= 0 && col >= 0){   // if row and col are provided, calculate offset
        offset = get_screen_offset(row, col);
    }else{                      // otherwise, use the current cursor position
        offset = get_cursor();
    }

    if(ch == '\n'){                                         // handle newline character
        int rows = offset / (CHARACTER_SIZE * MAX_COLS);    // calculate current row
        offset = get_screen_offset(rows, MAX_COLS - 1);     // set offset to the end of the row
    }else{
        vidmem[offset] = ch;                                // write the character to the video memory
        vidmem[offset + 1] = attribute_byte;                // write the attribute byte to the video memory
    }

    offset += CHARACTER_SIZE;

    offset = handle_scrolling(offset);
    set_cursor(offset);
}

int get_screen_offset(int row, int col)
{
    return (row * MAX_COLS + col) * CHARACTER_SIZE;
}

int get_cursor()
{
    int offset;

    // request high byte of cursor offset (data 14)
    port_byte_out(REG_SCREEN_CTRL, 14);
    offset = port_byte_in(REG_SCREEN_DATA) << 8;

    // request low byte of cursor offset (data 15)
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    
    return offset * CHARACTER_SIZE;
}

void set_cursor(int offset)
{
    offset /= CHARACTER_SIZE;

    // write high byte of cursor offset (data 14)
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    
    // write low byte of cursor offset (data 15)
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

void print_at(const char* message, int row, int col)
{
    if(row >= 0 && col >= 0){
        set_cursor(get_screen_offset(row, col));
    }

    int i = 0;
    while(message[i] != '\0'){
        print_char(message[i], row, col + i, WHITE_ON_BLACK);
        i++;
    }
}

void print(const char* message)
{
    print_at(message, -1, -1);
}

void clear_screen()
{
    int row = 0;
    int col = 0;

    // simply fill the screen with spaces
    for(row = 0; row < MAX_ROWS; row++){
        for(col = 0; col < MAX_COLS; col++){
            print_char(' ', row, col, WHITE_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}

int handle_scrolling(int cursor_offset)
{
    // if cursor offset is within the screen, return it
    if(cursor_offset < MAX_ROWS * MAX_COLS * CHARACTER_SIZE){
        return cursor_offset;
    }

    int i;
    
    // move all rows one row up
    for(i = 1; i < MAX_ROWS; i++){
        memcpy((char*) (get_screen_offset(i - 1, 0) + VIDEO_ADDRESS), 
               (char*) (get_screen_offset(i, 0) + VIDEO_ADDRESS), 
                    MAX_COLS * CHARACTER_SIZE);
    }

    // clear the last row
    char *last_line = (char*) (get_screen_offset(MAX_ROWS - 1, 0) + VIDEO_ADDRESS);
    for(i = 0; i < MAX_COLS * CHARACTER_SIZE; i++){
        last_line[i] = 0;
    }

    // move cursor one row up
    cursor_offset -= CHARACTER_SIZE * MAX_COLS;

    return cursor_offset;
}

void delete_last_char()
{
    int offset = get_cursor() - CHARACTER_SIZE;
 
    if(offset <= 0){
        return;
    }
 
    unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
    vidmem[offset] = ' ';
    vidmem[offset + 1] = WHITE_ON_BLACK;
    set_cursor(offset);
}