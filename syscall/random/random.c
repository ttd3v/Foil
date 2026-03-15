#include "random.h"
i32 random(ref buffer, u64 length){
        i32 r = 0;
        __asm__ volatile (
                "syscall\n"
                : "=a" (r)
                : "a" (318), "D" (buffer), "S" (length), "d" (0)
                : "memory", "rcx", "r11"
        );
        return r;
}
i32 randall(ref buffer, u64 length){
        u64 cursor = 0;
        while (cursor < length){
                i32 l = random(buffer,length-cursor);
                if(l < 0) return l;
                cursor += l;
        }
        return cursor;
}
