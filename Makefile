ASM = nasm
CC = gcc
LD = ld

all: build_dir image

image: build/image.img

DRIVERS := $(wildcard drivers/*.c)
DRIVER_OBJS := $(patsubst drivers/%.c, build/%.o, $(DRIVERS))

build/image.img: boot kernel
	dd if=/dev/zero of=build/image.img bs=512 count=2880
	dd if=build/boot.bin of=build/image.img conv=notrunc
	dd if=build/kernel.bin of=build/image.img seek=1 conv=notrunc

boot: build/boot.bin

build/boot.bin: bootloader/boot.asm
	$(ASM) -f bin -o $@ $<

kernel: build/kernel.bin

build/kernel.bin: build/kernel_entry.o build/kernel.o $(DRIVER_OBJS)
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

build/kernel_entry.o: kernel/kernel_entry.asm
	$(ASM) -f elf32 -o $@ $<

build/kernel.o: kernel/kernel.c
	$(CC) -fno-pie -ffreestanding -m32 -c $< -o $@

$(DRIVER_OBJS): build/%.o : drivers/%.c
	$(CC) -fno-pie -ffreestanding -m32 -c $< -o $@

build_dir:
	mkdir -p build

clean:
	rm -rf build