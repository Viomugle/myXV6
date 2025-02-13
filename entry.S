#include "asm.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"

.pa2align 2
.text
.global multiboot_header
multiboot_header:
  #define magic 0x1badb002
  #define flags 0
  .long magic
  .long flags
  .long (-magic-flags)

.global _start
_start =V2P_WO(entry)

.globl entry
entry:
  # Turn on page size extension for 4Mbyte pages
  movl    %cr4, %eax
  orl     $(CR4_PSE), %eax
  movl    %eax, %cr4
  # Set page directory
  movl    $(V2P_WO(entrypgdir)), %eax # entrypgdir在main.c里
  movl    %eax, %cr3
  # Turn on paging.
  movl    %cr0, %eax
  orl     $(CR0_PG|CR0_WP), %eax
  movl    %eax, %cr0
    
  mov $main, %eax # 因为paging已经设置好，所以main方法是虚拟地址是可行的
  jmp *%eax

.comm stack, KSTACKSIZE # stack标签占据KSTACKSIZE大小的空间