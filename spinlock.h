struct spinlock
{
    unit locked;
    char *name;
    struct cpu* cpu;
    uint psc[10];
};