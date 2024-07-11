[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

%define ENDL 0x0D, 0x0A

start:
    mov si, STARTING_MSG
    call puts

    mov [BOOT_DRIVE], dl

    mov bp, 0x9000          ; base pointer
    mov sp, bp              ; stack pointer

    call load_kernel        ; load the kernel from disk

    call switch_to_pm       ; should never return from here

    jmp $                   ; in case something goes wrong

%include "include/asm/disk_read.asm"
%include "include/asm/puts.asm"
%include "bootloader/gdt.asm"
%include "bootloader/switch_to_pm.asm"
%include "include/asm/puts32.asm"

[bits 16]

load_kernel:
    mov si, KERNEL_LOAD_MSG
    call puts

    mov bx, KERNEL_OFFSET
    mov dh, 50              ; number of sectors to read (50 * 512 = 25KB)
    mov dl, [BOOT_DRIVE]
    call disk_read

    ret

[bits 32]
BEGIN_PM:
    mov ebx, PROTECTED_MODE_MSG
    call puts32

    call KERNEL_OFFSET              ; call the kernel

    jmp $

; global variables
STARTING_MSG db "Booting in 16 bits real mode", ENDL, 0
PROTECTED_MODE_MSG db "Switched to 32 bits protected mode", ENDL, 0
KERNEL_LOAD_MSG db "Loading the kernel", ENDL, 0
BOOT_DRIVE db 0

times 510-($-$$) db 0           ; the program must be 512 bytes
dw 0xAA55                       ; magic number so BIOS knows it's a boot sector