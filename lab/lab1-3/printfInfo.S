#printfInfo.S 
.section .text
.globl _start
_start:
.code32
   

   movw $0x10,%ax
   movw %ax,%ds
   movw %ax,%es
   movw %ax,%ss
  
 
   movw $0x10,%ax
   movw %ax,%es
   mov $0xb8d22,%edi


   mov $program,%esi
   movw $38,%cx
   rep movsb

 loop0: jmp loop0
#.p2align 2
program:
    .byte 'W',0xc,'e',0xc,'l',0xc,'c',0xc,'o',0xc,'m',0xc,'e,0xc,' ',0xc,'t',0xc,'o',0xc,' ',0xc,'d',0xc,'i',0xc,'s',0xc,'k',0xc,'.',0xc,'.',0xc,'.',0xc
