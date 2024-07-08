; Read DH sectors to ES:BX from drive DL
disk_read:
    mov cx, 3       ; try at least 3 times

.try:
    push dx         ; save how many sectors should be read

    mov ah, 0x02    ; BIOS read sectors function
    mov al, dh      ; read DH sectors
    mov ch, 0x00    ; select cylinder 0
    mov dh, 0x00    ; select head 0
    mov cl, 0x02    ; start reading from sector 2 (the one after the boot sector)

    int 0x13        ; call BIOS interrupt

    jc disk_error   ; if carry flag is set, there was an error

    pop dx          ; restore how many sectors should have been read
    cmp dh, al      ; compare how many sectors were read to how many should have been read
    je .done        ; if they match, we're done

    dec cx          ; decrement the loop counter
    jnz .try

.done:
    jne disk_error  ; if they don't match, there was an error

    ret

; includes/asm/puts.asm should already be included
disk_error:
    mov si, DISK_ERROR_MSG
    call puts
    jmp $

DISK_ERROR_MSG db "Disk read error", ENDL, 0