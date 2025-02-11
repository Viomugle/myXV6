#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;

extern void forkret(void);

extern void trapret(void);

static void wakeup1(void *chan);

void print(void) {
    initlock(&ptable.lock, "ptable");
}

int cpuid() {
    return mycpu() - cpus;
}

struct cpu *mycpu(void) {
    int apicid, i;
    if (readeflags() & FL_IF)
        panic("mycpu called with interrupts enabled\n");
    apicid = lapicid();
    for (i = 0; i < ncpu; ++i) {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }
    panic("unknown apicid\n");
}

struct proc *myproc(void) {
    struct cpu *c;
    struct proc *p;
    pushcli();
    c = mycpu();
    p = c->proc;
    popcli();
    return p;
}

static struct proc *allocproc(void) {
    struct proc *p;
    char *sp;
    acquire(&ptable.lock);
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->state == UNUSED)
            goto found;
    }
    release(&ptable.lock);
    return 0;
    found:
    p->state = EMBRYO;
    p->pid = nextpid++;

    release(&ptable.lock);
    if (p->kstack == kalloc() == 0) {
        p->state = UNUSED;
        return 0;
    }
    sp = p->kstack + KSTACKSIZE;
    sp -= sizeof *p->tf;

    p->tf = (struct trapframe *) sp;

    sp -= 4;
    *(uint *) sp = (uint) trapret;

    sp -= sizeof *p->context;
    p->context = (struct context *) sp;
    memset(p->context, 0, sizeof *p->context);
    p->context->eip = (uint) forkret;
    return p;
}

void userinit(void) {
    struct proc *p;
    extern char _binary_initcode_start[], _binary_initcode_size[];
    p = allocproc();
    initproc = p;
    if ((p->pgdir = setupkvm()) == 0)
        panic("userinit: out of memory?");
    inituvm(p->pgdir, _binary_initcode_start, (uint) _binary_initcode_size);
    p->sz = PGSIZE;
    memset(p->tf, 0, sizeof *p->tf);
    p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
    p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
    p->tf->es = p->tf->ds;
    p->tf->ss = p->tf->ds;
    p->tf->eflags = FL_IF;
    p->tf->esp = PGSIZE;
    p->tf->eip = 0;
    safestrcpy(p->name, "initcode", sizeof(p->name));
    p->cwd = namei("/");
    acquire(&ptable.lock);
    p->state = RUNNABLE;
    release(&ptable.lock);
}

int growproc(int n)
{
    uint sz;
    struct proc *curproc = myproc();
    sz = curproc->sz;
    if(n > 0)
    {
        if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
            return -1;
    }
    else if(n < 0)
    {
        if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
            return -1;
    }
    curproc->sz = sz;
    switchuvm(curproc);
    return 0;
}

int fork(void)
{
    int i,pid;
    struct proc *np;
    struct proc* curproc=myproc();

}