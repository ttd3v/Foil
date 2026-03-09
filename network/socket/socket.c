#include "socket.h"

i32 socket(i32 addr_family, i32 socket_type, i32 protocol){
        i32 output = 0;
        __asm__ volatile (
        "syscall\n"
        : "=a"(output)
        : "a" (41), "D" (addr_family), "S" (socket_type), "d" (protocol)
        : "rcx", "r11", "memory"
        );
        return output;
}
