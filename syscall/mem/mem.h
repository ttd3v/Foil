#ifndef MEM_H
#define MEM_H
#include "../../typos.h"
static const ref MAP_FAIL = (void*)-1;

#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_SEM 0x8
#define PROT_NONE 0x0
#define PROT_GROWSDOWN 0x01000000
#define PROT_GROWSUP 0x02000000	
#define MAP_TYPE 0x0f		
#define MAP_FIXED 0x10
#define MAP_SHARED 0x1
#define MAP_ANONYMOUS 0x20		
#define MAP_POPULATE 0x008000
#define MAP_NONBLOCK 0x010000
#define MAP_STACK 0x020000
#define MAP_HUGETLB 0x040000
#define MAP_SYNC 0x080000
#define MAP_FIXED_NOREPLACE 0x100000
#define MAP_UNINITIALIZED 0x4000000

ref map(u64 size);
ref mmap(u64 addr_hint,u64 size, u64 prot, u64 flags, u64 fd, u64 offset);
void unmap(ref pointer, u64 size);
void cpy(void * dest, const void *  src, u64 count);
void movq(u64 * dest, const u64 src);
#endif
