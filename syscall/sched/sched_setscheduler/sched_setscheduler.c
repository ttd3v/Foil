#include "sched_setscheduler.h"
i32 sched_setscheduler(i32 pid, i32 policy, i32 *priority){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (144), "D" (pid), "S" (policy), "d" (priority)
                : "rcx", "r11", "memory"
        );
        return a;
}
