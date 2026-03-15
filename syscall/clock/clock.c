#include "clock.h"

i32 clock_settime(i32 clockid, const struct timespec *time){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (227), "D" (clockid), "S" (time)
                : "rcx", "r11", "memory"
        );
        return a;
}
i32 clock_gettime(i32 clockid,struct timespec *time){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (228), "D" (clockid), "S" (time)
                : "rcx", "r11", "memory"
        );
        return a;
}
i32 clock_getres(i32 clockid,struct timespec *time){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (229), "D" (clockid), "S" (time)
                : "rcx", "r11", "memory"
        );
        return a;
}
i32 clock_nanosleep(i32 clockid,i32 flags, struct timespec *request, struct timespec* remain){
        i32 a = 0;
        register struct timespec* r asm("r10") = remain;
        __asm__ volatile (
                "syscall"
                : "=a" (a)
                : "a" (230), "D" (clockid), "S" (flags), "d" (request), "r" (r)
                : "rcx", "r11", "memory"
        );
        return a;
}
