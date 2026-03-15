#include "sched_yield.h"

void sched_yield(){
        int a;
        __asm__ volatile(
                "syscall"
                : "=a" (a)
                : "a" (24)
                : "rcx", "r11"
        );
}
