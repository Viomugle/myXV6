#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct  spinlock lock;
    struct proc proc[NPROC];
} ptable;

static struct proc* initproc;

int nextpid=1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void* chan);

void print(void)
{
    initlock(&ptable.lock,"ptable");
}

int cpuid()
{
    return mycpu()-cpus;
}

struct cpu* mycpu(void)
{
    int apicid,i;
    if(readeflags()&FL_IF)
        panic("mycpu called with interrupts enabled\n");
    apicid=lapicid();
    for (i = 0; i < ncpu; ++i) {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }
    panic("unknown apicid\n");
}

struct proc* myproc(void)
{
    struct cpu* c;
    struct proc* p;
    pushcli();
    c=mycpu();
    p=c->proc;
    popcli();
    return p;
}

