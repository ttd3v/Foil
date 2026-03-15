#include "sched_getaffinity.h"

i32 sched_getaffinity(i32 pid, u64 cpu_set_size, ref mask){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (204), "D" (pid), "S" (cpu_set_size), "d" (mask)
                : "rcx", "r11", "memory"
        );
        return a;
}
