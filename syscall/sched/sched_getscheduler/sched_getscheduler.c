#include "sched_getscheduler.h"
i32 sched_getscheduler(i32 pid){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (145), "D" (pid)
                : "rcx", "r11", "rcx"
        );
        return a;
}
