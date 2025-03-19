cmake_minimum_required(VERSION 3.28)
project(MyXV6 LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ASM_COMPILER "/usr/bin/gcc")
enable_language(ASM)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -Wall")

add_executable(MyXV6 main.cpp
        asm.h
        memlayout.h
        mmu.h
        bootasm.S
        bootmain.c
        types.h
        x86.h
        elf.h
        proc.c
        proc.h
        proc.c
)
