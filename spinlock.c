#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

void initlock(struct spinlock* lk,char* name)
{
    lk->name=name;
    lk->locked=0;
    lk->cpu=0;
}

void acquire(struct spinlock* lk)
{
    pushcli();
    if(holding(lk))
    panic("acquire");
while(xchg(&lk->locked,1)!=0)
    ;   
__sync_synchronize();
lk->cpu=mycpu();
getcallperpcs(&lk,lk->pcs);
}