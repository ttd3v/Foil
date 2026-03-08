#include "close.h"
i32 close(i32 fd){
    i32 result;
    asm volatile(
        "syscall"
        : "=a"(result)
        : "a"(3), "D"(fd)
        : "rcx", "r11", "memory"
    );
    return result;
}
