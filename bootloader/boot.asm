[org 0x7C00]
[bits 16]

%define ENDL 0x0D, 0x0A

start:
    mov si, STARTING_MSG
    call puts

    mov bp, 0x9000          ; base pointer
    mov sp, bp              ; stack pointer

    call switch_to_pm       ; should never return from here

    jmp $                   ; in case something goes wrong

%include "includes/asm/puts.asm"
%include "includes/asm/gdt.asm"
%include "includes/asm/switch_to_pm.asm"
%include "includes/asm/puts32.asm"

BEGIN_PM:
    mov ebx, PROTECTED_MODE_MSG
    call puts32

    jmp $

; global variables
STARTING_MSG db "Booting in 16 bits real mode", ENDL, 0
PROTECTED_MODE_MSG db "Switched to 32 bits protected mode", ENDL, 0
BOOT_DRIVE db 0

times 510-($-$$) db 0           ; the program must be 512 bytes
dw 0xAA55                       ; magic number so BIOS knows it's a boot sector

; for testing purposes
times 256 dw 0x8123
times 256 dw 0x4567