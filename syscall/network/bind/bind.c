#include "bind.h"
i32 bind(i32 fd, struct sockaddr *addr, u64 socklen){
        i32 output = 0;
        __asm__ volatile (
        "syscall\n"
        : "=a"(output)
        : "a" (49), "D" (fd), "S" (addr), "d" (socklen)
        : "rcx", "r11", "memory"
        );
        return output;
}
