#include "ping.h"
#include "../stringify/stringify.h"

#include "ping.h"
#include "../stringify/stringify.h"

void ping(i64 number){
#if ACTIVE
    u8 buffer[33]; 
    i32 len = stringify_i64(number, (str)buffer);
    buffer[len++] = '\n';
    asm volatile (
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "syscall\n"
        :
        : "S" (buffer), "d" (len)
        : "rax", "rdi"
    );
#endif
}

