[bits 32]

global idt_flush

idt_flush:
    cli                 ; Disable interrupts

    push ebp
    mov ebp, esp        ; Move the stack pointer into EBP

    mov eax, [esp + 8]  ; Load the address of the IDT
    lidt [eax]          ; Load the IDT

    mov esp, ebp        ; Restore the stack pointer
    pop ebp

    sti                 ; Enable interrupts
    ret                 ; Return

%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli                     ; Disable interrupts
        push 0                  ; Push a dummy error code
        push %1                 ; Push the interrupt number
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:                      ; The error code is already on the stack
        cli                     ; Disable interrupts
        push %1                 ; Push the interrupt number
        jmp isr_common_stub 
%endmacro

%macro IRQ 2
    global irq%1
    irq%1:
        cli                     ; Disable interrupts
        push %2                 ; Push the irq number and the interrupt number
        jmp irq_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

ISR_NOERRCODE 128
ISR_NOERRCODE 177

extern isr_handler

isr_common_stub:
    push ds          ; Pushes the data segment
    mov eax, cr2     ; Move the CR2 register into EAX
    push eax         ; Push the address of the page fault
    pusha            ; Push all the general purpose registers

    ; set the data segment registers to the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp     ; Move the stack pointer into EAX
    push eax         ; Push the stack pointer for the C handler

    call isr_handler ; Call the C handler

    popa             ; Pop all the general purpose registers
    add esp, 4       ; Clean up the address of the page fault
    pop ds           ; Pop the data segment
    add esp, 8       ; Clean up the error code and the interrupt number

    sti              ; Enable interrupts
    iret             ; Return from the interrupt

extern irq_handler

irq_common_stub:
    push ds          ; Pushes the data segment
    mov eax, cr2     ; Move the CR2 register into EAX
    push eax         ; Push the address of the page fault
    pusha

    ; set the data segment registers to the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp     ; Move the stack pointer into EAX
    push eax         ; Push the stack pointer for the C handler

    call irq_handler ; Call the C handler

    popa             ; Pop all the general purpose registers
    add esp, 4       ; Clean up the address of the page fault
    pop ds           ; Pop the data segment
    add esp, 8       ; Clean up the error code and the interrupt number

    sti              ; Enable interrupts
    iret             ; Return from the interrupt