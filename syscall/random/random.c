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
i32 rand_vomit(ref buffer, u64 length){
        u64 seed = 0xFFAA5511AAUL;
        i32 o = random(&seed,8);
        if(o != 8) return o;
        
        u64 cursor = 0;
        while(cursor < length){
                u64 value = (cursor * length) ^ seed;
                u64 delta = length - cursor;
                if(delta < 8){
                        ((u8*)buffer)[cursor++] = (u8)value;
                }
                ((u64*)(buffer+cursor))[0] = value;
                cursor += 8;
        }

        return 0;
}
