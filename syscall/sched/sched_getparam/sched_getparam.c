#include "sched_getparam.h"
i32 sched_getparam(i32 pid, enum SchedPolicies* param){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (143), "D" (pid), "S" (param)
                : "rcx", "r11", "memory"
        );
        return a;
};
