#include "read.h"
i32 read(i32 fd, void* buf, u64 count){
    i32 result;
    asm volatile(
        "syscall"
        : "=a"(result)
        : "a"(0), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return result;
}
