#include <stdint.h>
struct spinlock
{
    unit locked;
    char *name;
    struct cpu* cpu;
    uint psc[10];
};