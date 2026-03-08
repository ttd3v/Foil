#include "hash.h"

#define PRIME1 11400714785074694791ULL
#define PRIME2 14029467366897019727ULL
#define PRIME3  1609587929392839161ULL
#define PRIME4  9650029242287828579ULL
#define PRIME5  2870177450012600261ULL

#define ROTL64(x, r) ((x << r) | (x >> (64 - r)))

static inline u64 round64(u64 acc, u64 input){
    acc += input * PRIME2;
    acc  = ROTL64(acc, 31);
    acc *= PRIME1;
    return acc;
}

static inline u64 merge_round(u64 acc, u64 val){
    val  = round64(0, val);
    acc ^= val;
    acc  = acc * PRIME1 + PRIME4;
    return acc;
}

u64 hash(ref input, u64 len, u64 seed){
    const u8* p   = (const u8*)input;
    const u8* end = p + len;
    u64 h64;

    if(len >= 32){
        u64 v1 = seed + PRIME1 + PRIME2;
        u64 v2 = seed + PRIME2;
        u64 v3 = seed;
        u64 v4 = seed - PRIME1;

        do {
            v1 = round64(v1, *(u64*)p); p += 8;
            v2 = round64(v2, *(u64*)p); p += 8;
            v3 = round64(v3, *(u64*)p); p += 8;
            v4 = round64(v4, *(u64*)p); p += 8;
        } while(p <= end - 32);

        h64 = ROTL64(v1, 1)  + ROTL64(v2, 7)
            + ROTL64(v3, 12) + ROTL64(v4, 18);

        h64 = merge_round(h64, v1);
        h64 = merge_round(h64, v2);
        h64 = merge_round(h64, v3);
        h64 = merge_round(h64, v4);
    } else {
        h64 = seed + PRIME5;
    }

    h64 += len;

    while(p <= end - 8){
        u64 k1 = round64(0, *(u64*)p);
        h64 ^= k1;
        h64  = ROTL64(h64, 27) * PRIME1 + PRIME4;
        p   += 8;
    }

    if(p <= end - 4){
        h64 ^= (u64)(*(u32*)p) * PRIME1;
        h64  = ROTL64(h64, 23) * PRIME2 + PRIME3;
        p   += 4;
    }

    while(p < end){
        h64 ^= (u64)(*p) * PRIME5;
        h64  = ROTL64(h64, 11) * PRIME1;
        p++;
    }

    h64 ^= h64 >> 33;
    h64 *= PRIME2;
    h64 ^= h64 >> 29;
    h64 *= PRIME3;
    h64 ^= h64 >> 32;

    return h64;
}
