ASM = nasm

all: build_dir image

image: build/image.img

build/image.img: boot kernel
	dd if=/dev/zero of=build/image.img bs=512 count=2880
	dd if=build/boot.bin of=build/image.img conv=notrunc
	dd if=build/kernel.bin of=build/image.img seek=1 conv=notrunc

boot: build/boot.bin

build/boot.bin: bootloader/boot.asm
	$(ASM) -f bin -o $@ $<

kernel: build/kernel.bin

build/kernel.bin: build/kernel_entry.o build/kernel.o 
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary	# 32 bit for intel 386 architecture	

build/kernel_entry.o: kernel/kernel_entry.asm
	$(ASM) -f elf32 -o $@ $<				 		# 32 bit

build/kernel.o: kernel/kernel.c
	gcc -fno-pie -ffreestanding -m32 -c $< -o $@	# no position independent code
													# freestanding - no standard library
													# 32 bit

build_dir:
	mkdir -p build