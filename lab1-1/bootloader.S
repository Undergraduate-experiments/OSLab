#bootloader.S (2020 mlx)

.section .text
.globl _start
_start:
.code16 
    movw %cs,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%ss

   #call
    call DispStr
loop0 : jmp loop0
DispStr:
     movw $BootMessage,%ax
     movw %ax,%bp
     movw $20,%cx
     movw $0x1004,%dx
     movw $0x000c,%bx
     movw $0x1301,%ax
     int $0x10
     ret
BootMessage:.ascii "OS Made By LuTong..."
.org 510
  .word 0xaa55

