#include "fcntl.h"

i32 fcntl(i32 fd, i32 cmd, i64 arg){
        i32 a;
        __asm__ __volatile__ (
                "syscall"
                : "=a" (a)
                : "a" (72), "D" (fd), "S" (cmd), "d" (arg)
                : "rcx", "r11"
        );
        return a;
}
