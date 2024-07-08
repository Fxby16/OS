; Print a string to the screen
; Parameters:
;   SI: pointer to the string
puts:
.start:
    ; save registers we'll be using
    push ax
.loop:
    lodsb               ; load the next byte from SI into AL
    or al, al           ; set the zero flag if AL is zero
    jz .done            ; jump to the end of the function if AL is zero
    mov ah, 0x0E        ; teletype output
    int 0x10            ; call the BIOS interrupt
    jmp .loop           ; repeat the process for the next character

.done:
    ; restore registers
    pop ax
    ret