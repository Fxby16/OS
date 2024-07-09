; Switch to 32 bit protected mode
[bits 16]
switch_to_pm:
    cli

    lgdt [gdt_descriptor]       ; load the GDT

    ; set the first bit of CR0 to 1 to make the switch to protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm        ; make a far jump to force the CPU to flush the pipeline

[bits 32]
init_pm:
    mov ax, DATA_SEG            ; set the data segment
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000            ; set the stack pointer
    mov esp, ebp                ; set the base pointer

    call BEGIN_PM