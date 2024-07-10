#include "../drivers/include/screen.h"
#include <stdio.h>

void _start()
{
    clear_screen();

    puts("Writing with puts() in kernel.c");
    printf("Writing with printf() in kernel.c\n");
    printf("Printing a number: %d\n", 42);
    printf("Printing a number with width 5: %5d\n", 42);
    printf("Printing a number with width 3: %3d\n", 42);
    printf("Printing a hex number: %x\n", 0x42);
    printf("Printing a string: %s\n", "Hello, World!");
    printf("Printing a string: %13s\n", "Test");
    printf("Printing a character: %c\n", 'A');

    while(1);
}