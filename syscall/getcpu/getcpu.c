#include "getcpu.h"
i32 getcpu(u32* current_cpu, u32* current_numa){
        i32 a = 0;
        __asm__ volatile (
                "syscall"
                :"=a"(a)
                :"a"(309), "D" (current_cpu), "S" (current_numa), "d" (-1)
        );
        return a;
}
