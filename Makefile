ASM = nasm

all: build_dir image

image: build/image.img

build/image.img: boot
	dd if=/dev/zero of=build/image.img bs=512 count=2880
	dd if=build/boot.bin of=build/image.img conv=notrunc

boot: build/boot.bin

build/boot.bin: bootloader/boot.asm
	$(ASM) -f bin -o $@ $<

build_dir:
	mkdir -p build