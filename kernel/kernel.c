#include "../drivers/include/screen.h"
#include "include/idt.h"
#include "include/ports_io.h"
#include "../drivers/include/keyboard.h"
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

void key_handler(struct keyboard_key* key, uint8_t flags)
{
    if(!key){
        return;
    }

    if((key->ascii >= 32 && key->ascii <= 126) || key->ascii == '\n' || key->ascii == '\t'){
        if((flags & SHIFT_DOWN) || (flags & CAPS_LOCK)){
            putchar(key->shift_ascii);
        }else{
            putchar(key->ascii);
        }
    }

    if(key->ascii == 8){ // backspace
        delete_last_char();
    }
}

void _start()
{
    clear_screen();
    idt_init();

    set_keyboard_handler(key_handler);

    while(1);
}