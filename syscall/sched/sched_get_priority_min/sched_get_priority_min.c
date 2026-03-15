#include "sched_get_priority_min.h"
i32 sched_get_priority_min(enum SchedPolicies policy){
        i32 r=0;
        __asm__ volatile(
                "syscall"
                :"=a"(r)
                :"a" (147), "D" (policy)
                :"rcx","r11","memory"
        );
        return r;
}

