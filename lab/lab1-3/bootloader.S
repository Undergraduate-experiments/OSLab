#bootloader.S 

.section .text
.globl _start
_start:
.code16
   
    movw %cs,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%ss

   #call
   call ClsSrn

   movw $0xb800,%ax
   movw %ax,%es
   movw $0xc82,%di
   movw $RmMessage,%si
   movw $38,%cx
   rep movsb
  
    #read disk
   movw $0x0000,%dx
   movw $0x0002,%cx
   movw $0x1000,%ax
   movw %ax,%es
   xor  %bx,%bx
   mov  $1,%al
   mov  $0x02,%ah
   int  $0x13


   cli
   movw $0x1000,%ax
   movw %ax,%ds
   xor  %ax,%ax
   movw %ax,%es
   movw $128,%cx
   sub  %si,%si  
   sub  %di,%di
   rep  movsd


   # call DispStr

   #RM TO PM
    xorw %ax,%ax
    movw %ax,%ds

   lgdt gdtdesc

   #open A20
  seta20.1:
      inb   $0x64,%al
      testb $0x2,%al
      jnz   seta20.1
      movb  $0xd1,%al
      outb  %al,$0x64
  seta20.2:
      inb   $0x64,%al
      testb $0x2,%al
      jnz   seta20.2
      movb  $0xdf,%al
      outb  %al,$0x60

   # set CR0 -- PE 1
   movl %cr0, %eax
   xor $0x1, %eax
   movl %eax, %cr0
   
   # go into PM
   ljmp $0x08,$0x0

ClsSrn:
     movw $0x0600, %ax
     movw $0x0700,%bx
     movw $0,%cx
     movw $0x0184f,%dx
     int $0x10
     ret

DispStr:
     movw $BootMessage,%ax
     movw %ax,%bp
     movw $20,%cx
     movw $0x1004,%dx
     movw $0x000c,%bx
     movw $0x1301,%ax   
     int $0x10
     ret

.p2align 2
gdt:
 .word 0,0,0,0

 #00000000 + FFFF  DS
 .word 0xFFFF
 .word 0x0000
 .word 0x9A00
 .word 0x00C0

 # CS
 .word 0xFFFF
 .word 0x0000
 .word 0x9200
 .word 0x00C0


gdtdesc:
 .word 0x7ff
 .long gdt

BootMessage: .ascii "OS made by LuTong ..."
#RmMessage:.ascii "Real Mode... : helloworld"
#PmMessage:.ascii "Protected Mode... : helloworld"
RmMessage:
    .byte 'R',0xc,'e',0xc,'a',0xc,'l',0xc,' ',0xc,'M',0xc,'o',0xc,'d',0xc,'e',0xc,' ',0xc,'h',0xc,'e',0xc,'l',0xc,'l',0xc,'w',0xc,'o',0xc,'r',0xc,'l',0xc,'d',0xc
.org 510
  .word 0xaa55

