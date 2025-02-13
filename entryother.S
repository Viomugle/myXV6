#include "asm.h"
#include "memlayout.h"
#include "mmu.h"

.code16
.global start
start:
cli
xorw %ax,%ax
movw %ax,%ds
movw %ax,%es
movw %ax,%ss

lgdt gdtdesc
movl %cr0,%eax
orl %CR0_PE,%eax
movl %eax,%cr0

ljmpl    $(SEG_KCODE<<3), $(start32)

.code 32
start32:
movw %cr0,%eax
orl %CR0_PE,%eax
mov %eax,$cr0

ljmpl    $(SEG_KCODE<<3), $(start32)

.code 32:
start32:
  movw    $(SEG_KDATA<<3), %ax    # Our data segment selector
  movw    %ax, %ds                # -> DS: Data Segment
  movw    %ax, %es                # -> ES: Extra Segment
  movw    %ax, %ss                # -> SS: Stack Segment
  movw    $0, %ax                 # Zero segments not ready for use
  movw    %ax, %fs                # -> FS
  movw    %ax, %gs                # -> GS

  # Switch to the stack allocated by startothers()
  movl    (start-4), %esp
  # Call mpenter()
  call	 *(start-8)

  movw    $0x8a00, %ax
  movw    %ax, %dx
  outw    %ax, %dx
  movw    $0x8ae0, %ax
  outw    %ax, %dx


  spin:
  jmp     spin

  .p2align 2
gdt:
  SEG_NULLASM
  SEG_ASM(STA_X|STA_R, 0, 0xffffffff)
  SEG_ASM(STA_W, 0, 0xffffffff)


gdtdesc:
  .word   (gdtdesc - gdt - 1)
  .long   gdt