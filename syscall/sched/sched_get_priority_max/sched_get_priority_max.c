#include "sched_get_priority_max.h"
i32 sched_get_priority_max(enum SchedPolicies policy){
        i32 r=0;
        __asm__ volatile(
                "syscall"
                :"=a"(r)
                :"a" (146), "D" (policy)
                :"rcx","r11","memory"
        );
        return r;
}
