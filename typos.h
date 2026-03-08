#ifndef TYPOS_H
#define TYPOS_H
typedef unsigned char u8;
typedef unsigned long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef u64 counter;
typedef struct {
        union{
                u64 num;
        };
        u8 vector[8];
} b64;


typedef char i8;
typedef long i64;
typedef int i32;
typedef short i16;
typedef double f64;
typedef float f32;
typedef void* mem;
typedef mem ref;
typedef char* str;
typedef ref exebin;

typedef struct {
        union{
                f64 num;
        };
        u8 vector[8];
} bf64;

_Static_assert(sizeof(u64) == 8,"u64 isn't 8 bytes");
_Static_assert(sizeof(u32) == 4,"u32 isn't 4 bytes");
_Static_assert(sizeof(u16) == 2,"u16 isn't 2 bytes");
_Static_assert(sizeof(u8) == 1,"u8 isn't 1 byte");
_Static_assert(sizeof(i64) == 8,"i64 isn't 8 bytes");
_Static_assert(sizeof(i32) == 4,"i32 isn't 4 bytes");
_Static_assert(sizeof(i16) == 2,"i16 isn't 2 bytes");
_Static_assert(sizeof(i8) == 1,"i8 isn't 1 byte");
_Static_assert(sizeof(f64) == 8,"f64 isn't 8 bytes");
_Static_assert(sizeof(f32) == 4,"f32 isn't 4 bytes");
_Static_assert(sizeof(mem) == sizeof(void*),"mem isn't void* size");
_Static_assert(sizeof(mem) == sizeof(ref),"ref isn't mem size");
_Static_assert(sizeof(mem) == sizeof(str),"str isn't mem size");

#endif
