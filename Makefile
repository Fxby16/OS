ASM = nasm
CC = gcc
LD = ld
CFLAGS = -ffreestanding -m32 -fno-pie -fno-stack-protector
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 --oformat binary
INCLUDES = -I libc/include

all: build_dir image

image: build/image.img

DRIVERS := $(wildcard drivers/*.c)
KERNEL_C := $(wildcard kernel/*.c)
KERNEL_ASM := $(wildcard kernel/*.asm)
KERNEL_ASM := $(filter-out kernel/kernel_entry.asm, $(KERNEL_ASM))
LIBC := $(wildcard libc/*.c)

DRIVER_OBJS := $(patsubst drivers/%.c, build/%.o, $(DRIVERS))
KERNEL_C_OBJS := $(patsubst kernel/%.c, build/%.o, $(KERNEL_C))
KERNEL_ASM_OBJS := $(patsubst kernel/%.asm, build/%.o, $(KERNEL_ASM))
LIBC_OBJS := $(patsubst libc/%.c, build/%.o, $(LIBC))

build/image.img: boot kernel
	dd if=/dev/zero of=build/image.img bs=512 count=2880
	dd if=build/boot.bin of=build/image.img conv=notrunc
	dd if=build/kernel.bin of=build/image.img seek=1 conv=notrunc

boot: build/boot.bin

build/boot.bin: bootloader/boot.asm
	$(ASM) -f bin -o $@ $<

kernel: kernel_objs build/kernel.bin

build/kernel.bin: build/kernel_entry.o  $(DRIVER_OBJS) $(LIBC_OBJS) $(KERNEL_C_OBJS) $(KERNEL_ASM_OBJS) 
	$(LD) $(LDFLAGS) -Ttext 0x1000  -o $@ $^

build/kernel_entry.o: kernel/kernel_entry.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

kernel_objs: $(KERNEL_ASM) $(KERNEL_C)
	$(CC) $(CFLAGS) -c $(KERNEL_C) -o $(KERNEL_C_OBJS) $(INCLUDES)
	@if [ -n "$(KERNEL_ASM)" ]; then \
        $(ASM) $(ASMFLAGS) -o $(KERNEL_ASM_OBJS) $(KERNEL_ASM); \
    fi

$(DRIVER_OBJS): build/%.o : drivers/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(LIBC_OBJS): build/%.o : libc/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

build_dir:
	mkdir -p build

clean:
	rm -rf build