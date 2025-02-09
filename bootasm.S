#include "asm.h"
#include "memlayout.h"
#include "mmu.h"
//code src dst
.code16
.globl start
start:
 cli
 xorw %ax, %ax #16bit size ,1 word = 2 bytes = 16 bits
 movw %ax,%ds
 movw %ax,%es
 movw %ax,%ss

seta20.1:
    inb $0x64,%al
    testb $2,%al
    jnz seta20.1

    movb $0xd1,%al
    outb %al,$0x64
seta20.2:
    inb $0x64,%al
    testb $2,%al
    jnz seta20.2

    movb $0xdf,%al
    outb %al,$0x60

    lgdt gdtdesc
    movl %cr0,%eax
    orl $CR0_PE_ON,%eax
    movl %eax,%cr0

    ljmp    $(SEG_KCODE<<3), $start32

.code32
start32:
    movw $(SEG_KDATA<<3), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs

    movl $start, %esp
    call bootmain

  movw    $0x8a00, %ax            # 0x8a00 -> port 0x8a00
  movw    %ax, %dx
  outw    %ax, %dx
  movw    $0x8ae0, %ax            # 0x8ae0 -> port 0x8a00
  outw    %ax, %dx
spin:
    jmp spin


.p2align 2                                # force 4 byte alignment
gdt:
  SEG_NULLASM                             # null seg,gdt的地一个descriptor不用
  SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # code seg
  SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg

gdtdesc:
  .word   (gdtdesc - gdt - 1)             # sizeof(gdt) - 1,gdt的地址范围是[base, base+limit],包含最后一个字节,所以减1
  .long   gdt                             # address gdt



