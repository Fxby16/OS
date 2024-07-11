#include "idt.h"
#include "ports_io.h"
#include <memory.h>
#include <stdio.h>

struct interrupt_descriptor idt[256];
struct idt_ptr idt_p;

void idt_init()
{
    memset(&idt, 0, sizeof(struct interrupt_descriptor) * 256);

    idt_p.limit = sizeof(struct interrupt_descriptor) * 256 - 1;
    idt_p.base = (uint32_t) &idt;

    // Begin remapping the Programmable Interrupt Controller (PIC)
    // ICW1: Start initialization in cascade mode
    port_byte_out(0x20, 0x11); // Send ICW1 to the master PIC command port
    port_byte_out(0xA0, 0x11); // Send ICW1 to the slave PIC command port

    // ICW2: Define the base addresses for the interrupt vectors
    port_byte_out(0x21, 0x20); // Set master PIC vector offset to 32 (0x20)
    port_byte_out(0xA1, 0x28); // Set slave PIC vector offset to 40 (0x28)

    // ICW3: Configure the PICs to work together in a master-slave setup
    port_byte_out(0x21, 0x04); // Tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    port_byte_out(0xA1, 0x02); // Tell slave PIC its cascade identity (2)

    // ICW4: Set operating mode of the PICs
    port_byte_out(0x21, 0x01); // Set master PIC to operate in 8086 mode
    port_byte_out(0xA1, 0x01); // Set slave PIC to operate in 8086 mode

    // Unmask all IRQs to allow the PICs to send interrupts
    port_byte_out(0x21, 0x0); // Enable all IRQs on master PIC
    port_byte_out(0xA1, 0x0); // Enable all IRQs on slave PIC

    // Set up the IDT entries
    // 0x08 is the code segment offset
    // 0x8E is the type and attributes
    idt_set_gate(0, isr0, 0x08, 0x8E);
    idt_set_gate(1, isr1, 0x08, 0x8E);
    idt_set_gate(2, isr2, 0x08, 0x8E);
    idt_set_gate(3, isr3, 0x08, 0x8E);
    idt_set_gate(4, isr4, 0x08, 0x8E);
    idt_set_gate(5, isr5, 0x08, 0x8E);
    idt_set_gate(6, isr6, 0x08, 0x8E);
    idt_set_gate(7, isr7, 0x08, 0x8E);
    idt_set_gate(8, isr8, 0x08, 0x8E);
    idt_set_gate(9, isr9, 0x08, 0x8E);
    idt_set_gate(10, isr10, 0x08, 0x8E);
    idt_set_gate(11, isr11, 0x08, 0x8E);
    idt_set_gate(12, isr12, 0x08, 0x8E);
    idt_set_gate(13, isr13, 0x08, 0x8E);
    idt_set_gate(14, isr14, 0x08, 0x8E);
    idt_set_gate(15, isr15, 0x08, 0x8E);
    idt_set_gate(16, isr16, 0x08, 0x8E);
    idt_set_gate(17, isr17, 0x08, 0x8E);
    idt_set_gate(18, isr18, 0x08, 0x8E);
    idt_set_gate(19, isr19, 0x08, 0x8E);
    idt_set_gate(20, isr20, 0x08, 0x8E);
    idt_set_gate(21, isr21, 0x08, 0x8E);
    idt_set_gate(22, isr22, 0x08, 0x8E);
    idt_set_gate(23, isr23, 0x08, 0x8E);
    idt_set_gate(24, isr24, 0x08, 0x8E);
    idt_set_gate(25, isr25, 0x08, 0x8E);
    idt_set_gate(26, isr26, 0x08, 0x8E);
    idt_set_gate(27, isr27, 0x08, 0x8E);
    idt_set_gate(28, isr28, 0x08, 0x8E);
    idt_set_gate(29, isr29, 0x08, 0x8E);
    idt_set_gate(30, isr30, 0x08, 0x8E);
    idt_set_gate(31, isr31, 0x08, 0x8E);

    idt_set_gate(32, irq0, 0x08, 0x8E);
    idt_set_gate(33, irq1, 0x08, 0x8E);
    idt_set_gate(34, irq2, 0x08, 0x8E);
    idt_set_gate(35, irq3, 0x08, 0x8E);
    idt_set_gate(36, irq4, 0x08, 0x8E);
    idt_set_gate(37, irq5, 0x08, 0x8E);
    idt_set_gate(38, irq6, 0x08, 0x8E);
    idt_set_gate(39, irq7, 0x08, 0x8E);
    idt_set_gate(40, irq8, 0x08, 0x8E);
    idt_set_gate(41, irq9, 0x08, 0x8E);
    idt_set_gate(42, irq10, 0x08, 0x8E);
    idt_set_gate(43, irq11, 0x08, 0x8E);
    idt_set_gate(44, irq12, 0x08, 0x8E);
    idt_set_gate(45, irq13, 0x08, 0x8E);
    idt_set_gate(46, irq14, 0x08, 0x8E);
    idt_set_gate(47, irq15, 0x08, 0x8E);

    idt_set_gate(128, isr128, 0x08, 0x8E); // System call
    idt_set_gate(177, isr177, 0x08, 0x8E); // System call

    idt_flush((uint32_t) &idt_p);   // Load the IDT
}

void idt_set_gate(uint8_t num, void* base, uint16_t selector, uint8_t flags)
{
    idt[num].base_low = ((uint32_t) base & 0xFFFF);
    idt[num].base_high = ((uint32_t) base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags | 0x60;
}

const char* error_messages[] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "x87 FPU floating-point error",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Control protection exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception",
    "Unknown exception"
};

void isr_handler(struct interrupt_registers* regs)
{
    if(regs->int_no < 32){
        printf("Exception: ");
        printf(error_messages[regs->int_no]);
        putchar('\n');
    }

    puts("Registers:");
    printf("CR2: %x\n", regs->cr2);
    printf("DS: %x\n", regs->ds);
    printf("EDI: %x\n", regs->edi);
    printf("ESI: %x\n", regs->esi);
    printf("EBP: %x\n", regs->ebp);
    printf("ESP: %x\n", regs->esp);
    printf("EBX: %x\n", regs->ebx);
    printf("EDX: %x\n", regs->edx);
    printf("ECX: %x\n", regs->ecx);
    printf("EAX: %x\n", regs->eax);
    printf("INT_NO: %x\n", regs->int_no);
    printf("ERR_CODE: %x\n", regs->err_code);
    printf("EIP: %x\n", regs->eip);
    printf("CS: %x\n", regs->cs);
    printf("EFLAGS: %x\n", regs->eflags);
    printf("USERESP: %x\n", regs->useresp);
    printf("SS: %x\n", regs->ss);

    puts("Halting...");

    while(1);
}

void* irq_routines[16] = {0};

void irq_set_handler(int irq, void (*handler)(struct interrupt_registers* regs))
{
    irq_routines[irq] = handler;
}

void irq_unset_handler(int irq)
{
    irq_routines[irq] = 0;
}

void irq_handler(struct interrupt_registers* regs)
{
    void (*handler)(struct interrupt_registers* regs) = irq_routines[regs->int_no - 32];
    
    if(handler){
        handler(regs);
    }

    if(regs->int_no >= 40){
        port_byte_out(0xA0, 0x20); // Send EOI to slave PIC
    }

    port_byte_out(0x20, 0x20); // Send EOI to master PIC
}