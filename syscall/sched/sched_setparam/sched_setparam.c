#include "sched_setparam.h"
i32 sched_setparam(i32 pid,const enum SchedPolicies* param){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (142), "D" (pid), "S" (param)
                : "rcx", "r11", "memory"
        );
        return a;
};
