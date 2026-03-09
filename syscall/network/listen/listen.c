#include "listen.h"
i32 listen(i32 fd, i32 backlog){
        i32 output = 0;
        __asm__ volatile (
        "syscall\n"
        : "=a"(output)
        : "a" (41), "D" (backlog)
        : "rcx", "r11", "memory"
        );
        return output;
}
