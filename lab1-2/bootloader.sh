as -o bootloader.o bootloader.S
ld -Ttext 0x7c00 -o bootloader.elf bootloader.o
objcopy -j .text -O binary bootloader.elf bootloader.com
dd if=bootloader.com of=bootloader.img bs=512 count=1 conv=notrunc
bochs -q -f bochsrc
