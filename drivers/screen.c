#include "include/screen.h"

// private functions

int get_screen_offset(int row, int col);
int get_cursor();
void set_cursor(int offset);
void memory_copy(char *source, char *dest, int no_bytes);
int handle_scrolling(int cursor_offset);

void port_byte_out(unsigned short port, unsigned char data);
unsigned char port_byte_in(unsigned short port);

void print_char(char ch, int row, int col, char attribute_byte)
{
    unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
    
    if(!attribute_byte){
        attribute_byte = WHITE_ON_BLACK;
    }

    int offset;
    if(row >= 0 && col >= 0){
        offset = get_screen_offset(row, col);
    } else{
        offset = get_cursor();
    }

    if(ch == '\n'){
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(rows, MAX_COLS - 1);
    }else{
        vidmem[offset] = ch;
        vidmem[offset + 1] = attribute_byte;
    }

    offset += 2;

    offset = handle_scrolling(offset);
    set_cursor(offset);
}

int get_screen_offset(int row, int col)
{
    return (row * MAX_COLS + col) * 2;
}

int get_cursor()
{
    int offset;

    port_byte_out(REG_SCREEN_CTRL, 14);
    offset = port_byte_in(REG_SCREEN_DATA) << 8;

    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    
    return offset * 2;
}

void set_cursor(int offset)
{
    offset /= 2;

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

void print_at(char* message, int row, int col)
{
    if(row >= 0 && col >= 0){
        set_cursor(get_screen_offset(row, col));
    }

    int i = 0;
    while(message[i] != '\0'){
        print_char(message[i++], row, col + i, WHITE_ON_BLACK);
    }
}

void print(char* message)
{
    print_at(message, -1, -1);
}

void clear_screen()
{
    int row = 0;
    int col = 0;

    for(row = 0; row < MAX_ROWS; row++){
        for(col = 0; col < MAX_COLS; col++){
            print_char(' ', row, col, WHITE_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}

void memory_copy(char* source, char* dest, int no_bytes)
{
    int i;
    for(i = 0; i < no_bytes; i++){
        *(dest + i) = *(source + i);
    }
}

int handle_scrolling(int cursor_offset)
{
    if(cursor_offset < MAX_ROWS * MAX_COLS * 2){
        return cursor_offset;
    }

    int i;
    for(i = 1; i < MAX_ROWS; i++){
        memory_copy((char*) (get_screen_offset(i, 0) + VIDEO_ADDRESS), (char*) (get_screen_offset(i - 1, 0) + VIDEO_ADDRESS), MAX_COLS * 2);
    }

    char *last_line = (char*) (get_screen_offset(MAX_ROWS - 1, 0) + VIDEO_ADDRESS);
    for(i = 0; i < MAX_COLS * 2; i++){
        last_line[i] = 0;
    }

    cursor_offset -= 2 * MAX_COLS;

    return cursor_offset;
}

void port_byte_out(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}