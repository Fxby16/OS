; call the kernel. this ensures that the kernel is called when jumping to where the kernel is loaded

[bits 32]
[extern main]

call main

jmp $