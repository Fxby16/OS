#include "include/keyboard.h"
#include "../kernel/include/ports_io.h"
#include <stdio.h>

// http://www.osfree.org/docs/cmdref/cmdref.2.0476.php
// scancode can be used as index until 53 (index = scancode - 1)
struct keyboard_key us_layout[] = {
    {1, 27, 27},
    {2, 49, 33},
    {3, 50, 64},
    {4, 51, 35},
    {5, 52, 36},
    {6, 53, 37},
    {7, 54, 94},
    {8, 55, 38},
    {9, 56, 42},
    {10, 57, 40},
    {11, 48, 41},
    {12, 45, 95},
    {13, 61, 43},
    {14, 8, 8},
    {15, 9, 9},
    {16, 113, 81},
    {17, 119, 87},
    {18, 101, 69},
    {19, 114, 82},
    {20, 116, 84},
    {21, 121, 89},
    {22, 117, 85},
    {23, 105, 73},
    {24, 111, 79},
    {25, 112, 80},
    {26, 91, 123},
    {27, 93, 125},
    {28, 10, 10},
    {29, 17, 17},
    {30, 97, 65},
    {31, 115, 83},
    {32, 100, 68},
    {33, 102, 70},
    {34, 103, 71},
    {35, 104, 72},
    {36, 106, 74},
    {37, 107, 75},
    {38, 108, 76},
    {39, 59, 58},
    {40, 39, 34},
    {41, 96, 126},
    {42, 16, 16},
    {43, 92, 124},
    {44, 122, 90},
    {45, 120, 88},
    {46, 99, 67},
    {47, 118, 86},
    {48, 98, 66},
    {49, 110, 78},
    {50, 109, 77},
    {51, 44, 60},
    {52, 46, 62},
    {53, 47, 63},

    {57, 32, 32},
    {59, 0, 0},
    {60, 0, 0},
    {61, 0, 0},
    {62, 0, 0},
    {63, 0, 0},
    {64, 0, 0},
    {65, 0, 0},
    {66, 0, 0},
    {67, 0, 0},
    {68, 0, 0},
    {133, 0, 0},
    {134, 0, 0},
    {55, 42, 42},
    {71, 224, 55},
    {72, 224, 56},
    {73, 224, 57},
    {74, 45, 45},
    {75, 224, 52},
    {76, 0, 53},
    {77, 224, 54},
    {78, 43, 43},
    {79, 224, 49},
    {80, 224, 50},
    {81, 224, 51},
    {82, 224, 48},
    {83, 224, 46},
    {224, 13, 47}
};

#define SCANCODE_RELEASED 0x80
#define LEFT_SHIFT 42
#define RIGHT_SHIFT 54
#define CAPS_LOCK_ 58
#define NUM_LOCK_ 69

uint8_t flags = 0;
void (*user_handler)(struct keyboard_key* key, uint8_t flags);

struct keyboard_key* find_key(uint8_t scancode)
{
    for(int i = 53; i < sizeof(us_layout) / sizeof(struct keyboard_key); i++){
        if(us_layout[i].scancode == scancode){
            return &us_layout[i];
        }
    }

    return 0;   //key not found (should never happen)
}

void keyboard_handler(struct interrupt_registers* regs)
{
    uint8_t scancode = port_byte_in(0x60);
    
    //check if shift key is pressed
    if((scancode == LEFT_SHIFT || scancode == (LEFT_SHIFT | SCANCODE_RELEASED)) || 
      ((scancode == RIGHT_SHIFT || scancode == (RIGHT_SHIFT | SCANCODE_RELEASED)))){
        
        if(scancode & SCANCODE_RELEASED){
            flags &= (ALL_FLAGS ^ SHIFT_DOWN);
        }else{
            flags |= SHIFT_DOWN;
        }
    }

    if(scancode == CAPS_LOCK_){
        if(!(scancode & SCANCODE_RELEASED)){     //key pressed
            flags ^= CAPS_LOCK;
        }
    }

    if(scancode == NUM_LOCK_){
        if(!(scancode & SCANCODE_RELEASED)){     //key pressed
            flags ^= NUM_LOCK;
        }
    }

    flags &= (ALL_FLAGS ^ KEY_RELEASED);
    if(scancode & SCANCODE_RELEASED){
        flags |= KEY_RELEASED;
    }

    struct keyboard_key* key = 0;

    if(scancode < 54){
        key = &us_layout[scancode - 1];
    }else{
        key = find_key(scancode);
    }

    if(user_handler){
        user_handler(key, flags);
    }
}

void set_keyboard_handler(void (*handler)(struct keyboard_key* key, uint8_t flags))
{
    user_handler = handler;

    irq_set_handler(1, keyboard_handler);
}