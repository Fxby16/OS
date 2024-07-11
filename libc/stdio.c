#include <stdarg.h>
#include <minmax.h>
#include <string.h>
#include <memory.h>
#include "../drivers/include/screen.h"

// printf state machine states
enum PRINTF_STATE{
    PRINTF_NORMAL,
    PRINTF_PERCENT
};

void puts(const char* str);
void putchar(char ch);
void print_dec(int num, int width);
void print_udec(unsigned int num, int width);
void print_hex(int num);

// flags not implemented
// precision not implemented since floating point numbers are not supported yet
// length not implemented
void printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);

    enum PRINTF_STATE state = PRINTF_NORMAL;
    int width = 0;
    char* str_ptr = 0;

    while(*str){
        switch(state){
            case PRINTF_NORMAL:
                if(*str == '%'){
                    state = PRINTF_PERCENT;
                    str++;
                }else{
                    putchar(*str);
                    str++;
                }
                break;
            case PRINTF_PERCENT:
                if(*str >= '0' && *str <= '9'){
                    while(*str >= '0' && *str <= '9'){
                        width = width * 10 + (*str - '0');
                        str++;
                    }
                }else{
                    switch(*str){
                        case 'c':
                            for(int i = 0; i < max(0, width - 1); i++){ // pad until minimum width is reached
                                putchar(' ');
                            }
                            putchar(va_arg(args, int));
                            break;
                        case 'd':
                            print_dec(va_arg(args, int), width);
                            break;
                        case 'u':
                            print_udec(va_arg(args, unsigned int), width);
                            break;
                        case 's':
                            str_ptr = va_arg(args, char*);
                            for(int i = 0; i < max(0, width - strlen(str_ptr)); i++){ // pad until minimum width is reached
                                putchar(' ');
                            }
                            
                            while(*str_ptr){
                                putchar(*str_ptr);
                                str_ptr++;
                            }

                            break;
                        case 'x':
                            print_hex(va_arg(args, int));
                            break;
                        default:
                            puts("Unknown format specifier");
                            break;
                    }

                    state = PRINTF_NORMAL;

                    str++;
                }
                break;
        }
    }

    va_end(args);
}

void puts(const char* str)
{
    print(str);
    print("\n");
}

void putchar(char ch)
{
    print_char(ch, -1, -1, WHITE_ON_BLACK);
}

void print_dec(int num, int width)
{
    char str[15];
    int neg = num < 0;

    if(neg){
        num = -num;
    }

    memset(str, 0, sizeof(str));

    int i = 0;
    while(num){
        str[i] = num % 10 + '0';
        num /= 10;
        i++;
    }

    if(i == 0){
        str[i] = '0';
        i++;
    }

    if(neg){
        str[i] = '-';
        i++;
    }

    for(int j = 0; j < max(0, width - i); j++){ // pad until minimum width is reached
        putchar(' ');
    }

    while(i){
        i--;
        putchar(str[i]);
    }
}

void print_udec(unsigned int num, int width)
{
    char str[15];

    memset(str, 0, sizeof(str));

    int i = 0;
    while(num){
        str[i] = num % 10 + '0';
        num /= 10;
        i++;
    }

    if(i == 0){
        str[i] = '0';
        i++;
    }

    for(int j = 0; j < max(0, width - i); j++){ // pad until minimum width is reached
        putchar(' ');
    }

    while(i){
        i--;
        putchar(str[i]);
    }
}

void print_hex(int num)
{
    char str[15];

    memset(str, 0, sizeof(str));

    int i = 0;
    while(num){
        int rem = num % 16;
        str[i] = (rem < 10) ? (rem + '0') : (rem - 10 + 'A');
        num /= 16;
        i++;
    }

    if(i == 0){
        str[i] = '0';
        i++;
    }

    while(i){
        i--;
        putchar(str[i]);
    }
}