VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Prints a null terminated string to the screen (in 32 bit protected mode)
; Parameters:
;   EBX: pointer to the string
puts32:
    pusha
    mov edx, VIDEO_MEMORY

.loop:
    mov al, [ebx]           ; store the next char in AL
    mov ah, WHITE_ON_BLACK  ; store the color attribute in AH

    cmp al, 0               ; check if we reached the end of the string
    je .done                

    mov [edx], ax           ; write the character to the video memory
    inc ebx                 ; move to the next character
    add edx, 2              ; move to the next position in video memory

    jmp .loop

.done:
    popa
    ret