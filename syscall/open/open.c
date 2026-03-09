#include "open.h"
i32 open(str pathname, u64 flags, u64 mode){
    i32 result;
    asm volatile(
        "syscall"
        : "=a"(result)
        : "a"(2), "D"(pathname), "S"(flags), "d"(mode)
        : "rcx", "r11", "memory"
    );
    return result;
}
