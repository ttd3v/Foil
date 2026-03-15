#include "print.h"

void print(str string, u64 size){
        asm volatile (
                "mov $1, %%rax\n"
                "mov $1, %%rdi\n"
                "syscall\n"
                :
                : "S" (string), "d" (size)
                : "rax", "rdi"
        );
}
