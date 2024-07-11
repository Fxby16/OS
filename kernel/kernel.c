#include "../drivers/include/screen.h"
#include "idt.h"
#include <stdio.h>
#include <stdint.h>

void trigger_divide_by_zero()
{
    int n = 10 / 0;
}

void trigger_breakpoint()
{
    __asm__ __volatile__("int $3");
}

void trigger_bounds_check() {
    int array[2] = {0, 0};
    __asm__ __volatile__(
        "movl $3, %%eax\n"
        "bound %%eax, (%0)\n"
        :
        : "r" (array)
        : "%eax"
    );
}

void trigger_invalid_opcode() {
    __asm__ __volatile__("ud2");  // Undefined instruction
}

void trigger_device_not_available() {
    __asm__ __volatile__(
        "mov %cr0, %eax\n"
        "or $0x4, %eax\n"
        "mov %eax, %cr0\n"
        "finit\n"
    );
}

void _start()
{
    clear_screen();
    printf("Initializing the IDT\n");
    idt_init();
    printf("IDT initialized\n");

    puts("Writing with puts() in kernel.c");
    printf("Writing with printf() in kernel.c\n");
    printf("Printing a number: %d\n", -42);
    printf("Printing a number with width 5: %5d\n", 42);
    printf("Printing a number with width 3: %3d\n", 42);
    printf("Printing a hex number: %x\n", 0x42);
    printf("Printing an unsigned number: %u\n", UINT32_MAX);
    printf("Printing a string: %s\n", "Hello, World!");
    printf("Printing a string: %13s\n", "Test");
    printf("Printing a character: %c\n", 'A');

    trigger_divide_by_zero();

    while(1);
}