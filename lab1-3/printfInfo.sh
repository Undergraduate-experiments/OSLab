as -o printfInfo.o printfInfo.S
ld -Ttext 0x0 -o printfInfo.elf printfInfo.o
objcopy -j .text -O binary printfInfo.elf prob.com
