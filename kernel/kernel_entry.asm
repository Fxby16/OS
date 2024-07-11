; call the kernel. this ensures that the kernel is called when jumping to where the kernel is loaded

%define ENDL 0x0D, 0x0A

[bits 32]
[extern _start]

kernel_entry:
    mov ebx, CALLING_KERNEL_MSG
    call puts32

    call _start

    mov ebx, RETURNED_FROM_KERNEL_MSG
    call puts32

    jmp $

%include "include/asm/puts32.asm"
CALLING_KERNEL_MSG db "Calling the kernel...", ENDL, 0
RETURNED_FROM_KERNEL_MSG db "Returned from the kernel. Halting", ENDL, 0