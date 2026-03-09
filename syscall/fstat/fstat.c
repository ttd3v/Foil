#include "fstat.h"

i32 fstat(i32 fd, struct stat* buf){
    i32 result;
    asm volatile(
        "syscall"
        : "=a"(result)
        : "a"(5), "D"(fd), "S"(buf)
        : "rcx", "r11", "memory"
    );
    return result;
}
