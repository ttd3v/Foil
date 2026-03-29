#include "mem.h"
#include "../../typos.h"
void* map(u64 size);
void unmap (ref pointer, u64 size);
void cpy(void *dest, const void *src, u64 count);

inline void* map(u64 size) {
    void* addr;
    register long r10 __asm__("r10") = (MAP_ANONYMOUS | MAP_POPULATE | MAP_PRIVATE);
    register long r8  __asm__("r8")  = -1;
    register long r9  __asm__("r9")  =  0;
    __asm__ __volatile__ (
        "syscall\n"
        : "=a" (addr)
        : "a" (9), "D" (0), "S" (size), "d" (3), "r"(r10), "r"(r8), "r"(r9)
        : "r11", "rcx", "memory"
    );
    return addr;
}
inline void* mmap(u64 addr_hint,u64 size, u64 prot, u64 flags, u64 fd, u64 offset) {
    register long r10 __asm__("r10") = flags;
    register long r8  __asm__("r8")  = fd;
    register long r9  __asm__("r9")  = offset;
    void* addr;
    __asm__ __volatile__ (
        "syscall\n"
        : "=a" (addr)
        : "a" (9), "D" (addr_hint), "S" (size), "d" (prot), "r"(r10), "r"(r8), "r"(r9)
        : "r11", "rcx", "memory"
    );
    return addr;
}
inline void unmap(ref pointer, u64 size){
        __asm__ __volatile__ (
                "syscall\n"
                :
                : "a" (11), "D" (pointer), "S" (size)
                : "r11", "rcx", "memory"
        );
}
inline void cpy(void * restrict dest, const void * restrict src, u64 count){
        __asm__ volatile (
                "rep movsb\n"
                :
                : "S" (src), "D" (dest), "c" (count)
                : "memory"
        );
}
inline void movq(u64 * restrict dest, const u64 src){
    *dest = src;
}

