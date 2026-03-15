#include "sched_setaffinity.h"

i32 sched_setaffinity(i32 pid, u64 cpu_set_size, ref mask){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (203), "D" (pid), "S" (cpu_set_size), "d" (mask)
                : "rcx", "r11", "memory"
        );
        return a;
}
