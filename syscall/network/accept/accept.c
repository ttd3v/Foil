#include "accept.h"

i32 accept(i32 sockfd, struct sockaddr *addr, u64 *addrlen){
        i32 a;
        __asm__ __volatile__ (
                "syscall"
                : "=a" (a)
                : "a" (43), "D" (sockfd), "S" (addr), "d" (addrlen)
                : "rcx", "r11", "memory"
        );
        return a;
}
