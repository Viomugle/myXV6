//
// Created by 24510 on 2025/1/15.
//

#ifndef MYXV6_MEMLAYOUT_H
#define MYXV6_MEMLAYOUT_H

#define EXTMEM 0x100000
#define PHYSTOP 0xE000000
#define DEVPACE xFE000000

#define KERNBASE 0x80000000
#define KERNLINK (KERNBASE+EXTMEM)



#define V2P(a) (((unit)(a))-KERNBASE)
#define P2V(a) ((void*)(((char*)(a))+KERNBASE))




#endif //MYXV6_MEMLAYOUT_H
