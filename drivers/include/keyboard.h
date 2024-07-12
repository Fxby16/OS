#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../kernel/include/idt.h"
#include <stdint.h>

struct keyboard_key{
    uint8_t scancode;
    char ascii;
    char shift_ascii;
};

// key flags
#define SHIFT_DOWN 0x01
#define CAPS_LOCK 0x02
#define NUM_LOCK 0x04
#define KEY_RELEASED 0x08

#define ALL_FLAGS (SHIFT_DOWN | CAPS_LOCK | NUM_LOCK | KEY_RELEASED)

void keyboard_handler(struct interrupt_registers* regs);

/* Flags:
    0x01: Shift Down
    0x02: Caps Lock
    0x04: Num Lock
    0x08: Released (if 0 the key has just been pressed)
*/
void set_keyboard_handler(void (*handler)(struct keyboard_key* key, uint8_t flags));

#endif