#ifndef ARENA_H
#define ARENA_H
#include "../typos.h"

typedef struct {
        ref array[3];
        u32 length[3];
        u32 lock[3];
        u32 capacity[3];
        u32 flags;
} Arena;
_Static_assert(sizeof(Arena) == 64, "Wrong");
#endif
