#include "mkdir.h"
i32 mkdir(str pathname, u64 mode){
    i32 result;
    asm volatile(
        "syscall"
        : "=a"(result)
        : "a"(83), "D"(pathname), "S"(mode)
        : "rcx", "r11", "memory"
    );
    return result;
}
