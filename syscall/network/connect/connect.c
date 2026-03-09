#include "connect.h"
i32 connect(i32 fd, struct sockaddr* socket_addr, u64 addr_len){
        i32 r = 42;
        __asm__ volatile(
                "syscall\n"
                : "=a" (r)
                : "a" (r), "D" (fd), "S" (socket_addr), "d" (addr_len)
                : "rcx","r11","memory"
        );
        return r;
}
