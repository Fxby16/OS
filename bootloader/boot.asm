[org 0x7C00]
[bits 16]

%define ENDL 0x0D, 0x0A

start:
    mov [BOOT_DRIVE], dl    ; BIOS stores the boot drive in dl
    
    mov si, STARTING_MESSAGE
    call puts

    ; Set up the stack
    mov ax, 0x7C00  
    mov ss, ax              ; stack segment
    mov sp, 0x8000          ; stack pointer

    mov bx, 0x9000          ; disk read destination
    mov dh, 5               ; sectors to read
    mov dl, [BOOT_DRIVE]    ; boot drive
    call disk_read

    mov dx, [0x9000]        ; print the first word read from the first sector
    call hex_to_str

    mov si, HEX_OUT
    call puts

    mov dx, [0x9000 + 512]  ; print the first word read from the second sector
    call hex_to_str

    mov si, HEX_OUT
    call puts

    jmp $

%include "includes/asm/puts.asm"
%include "includes/asm/hex_to_str.asm"
%include "includes/asm/disk_read.asm"

; global variables
STARTING_MESSAGE db "Booting OS", ENDL, 0
BOOT_DRIVE db 0

times 510-($-$$) db 0           ; the program must be 512 bytes
dw 0xAA55                       ; magic number so BIOS knows it's a boot sector

; for testing purposes
times 256 dw 0x8123
times 256 dw 0x4567