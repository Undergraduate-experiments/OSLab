as -o bootloader.o bootloader.S
ld -Ttext 0x7c00 -o bootloader.elf bootloader.o
objcopy -j .text -O binary bootloader.elf bootloader.com
