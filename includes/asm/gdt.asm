; GDT (Global Descriptor Table)
gdt_start:                  ; the mandatory null descriptor
    dd 0x0                  ; dd = define double word (4 bytes)
    dd 0x0

gdt_code:                   ; the code segment descriptor
    ; base = 0x0, limit = 0xFFFFF
    ; 1st flags: present (1), privilege level (00), descriptor type (1) -> 1001b
    ; type flags: code segment (1), conforming (0), read/write (1), accessed (0) -> 1010b
    ; 2nd flags: granularity (1), 32-bit default (1), 64-bit segment (0), AVL (0) -> 1100b

    dw 0xFFFF               ; limit (bits 0-15)
    dw 0x0                  ; base (bits 0-15)
    db 0x0                  ; base (bits 16-23)
    db 10011010b            ; 1st flags, type flags
    db 11001111b            ; 2nd flags, limit (bits 16-19)
    db 0x0                  ; base (bits 24-31)

gdt_data:                   ; the data segment descriptor
    ; same as the code segment, but the type flags are different
    ; type flags: data segment (0), expand down (0), read/write (1), accessed (0) -> 0010b

    dw 0xFFFF               ; limit (bits 0-15)
    dw 0x0                  ; base (bits 0-15)
    db 0x0                  ; base (bits 16-23)
    db 10010010b            ; 1st flags, type flags
    db 11001111b            ; 2nd flags, limit (bits 16-19)
    db 0x0                  ; base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; the size of the GDT
    dd gdt_start               ; the address where the GDT starts

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start