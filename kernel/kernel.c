#include "../drivers/include/screen.h"

void main()
{
    clear_screen();

    print("Hello, World!\n");

    print_at("Test print at", 10, 10);

    print_char('X', 15, 0, WHITE_ON_BLACK);
}