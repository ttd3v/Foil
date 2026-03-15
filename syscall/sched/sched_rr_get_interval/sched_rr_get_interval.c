#include "sched_rr_get_interval.h"
i32 sched_rr_get_interval(i32 pid, struct timespec*time){
        i32 r=0;
        __asm__ volatile (
                "syscall"
                : "=a" (r)
                : "a" (148), "D" (pid), "S"(time)
                : "rcx", "r11", "memory"
        );
        return r;
};

