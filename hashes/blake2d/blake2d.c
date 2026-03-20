#include "blake2d.h"
#include "../../syscall/mem/mem.h"

const u8 sigma[12][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
        { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
        { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
        { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
        { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
        { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
        { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
        { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
        { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
};
typedef u64 HT[8];
const HT IV = {
        0x6A09E667F3BCC908, 
        0xBB67AE8584CAA73B,
        0x3C6EF372FE94F82B,
        0xA54FF53A5F1D36F1,
        0x510E527FADE682D1,
        0x9B05688C2B3E6C1F,
        0x1F83D9ABFB41BD6B,
        0x5BE0CD19137E2179
};

// BLAKE2b type legend
//
// d        -> u8*        input message (array of 128-byte blocks)
// dd       -> u64        number of 128-byte blocks in d
// ll       -> u64        message length in bytes
// kk       -> u64        key length in bytes (0 if unkeyed, max 64)
// nn       -> u64        desired output length in bytes (1-64)
//
// F(h, m, t, f)
// h        -> u64[8]     current state (8 x 64-bit words)
// m        -> u64[16]    current 128-byte message block as 16 x 64-bit words
// t        -> u64        counter — total bytes compressed so far
// f        -> bool       finalization flag, true on last block
//
// G(v, a, b, c, d, x, y)
// v        -> u64[16]    working vector (4x4 matrix of 64-bit words)
// a,b,c,d  -> u64        indices into v (not values, indices)
// x,y      -> u64        message words fed into this G call
//

#define ROR64(x,n) (((x) >> (n)) | ((x) << (64-(n))))
#define w 64
#define r 12
#define bb 128

#define R1 32
#define R2 24
#define R3 16
#define R4 63

static inline void G(u64 v[16], u64 a, u64 b, u64 c, u64 d, u64 x, u64 y){
        v[a] = (v[a]+v[b]+x);
        v[d] = ROR64((v[d]^v[a]), R1);
        v[c] = (v[c]+v[d]);
        v[b] = ROR64((v[b]^v[c]),R2);
        v[a] = (v[a] + v[b] + y);
        v[d] = ROR64((v[d] ^ v[a]),R3);
        v[c] = (v[c] + v[d]);
        v[b] = ROR64((v[b] ^ v[c]), R4);
}

static inline void F(HT *h, u64 m[16], u64 t, u64 f){
        u64 v[16];
        cpy(v, h, 64);
        cpy(v+8, IV, 64);
        v[12] ^= t;
        v[13] ^= ((t >> 63) >> 1);
        if(f==1) v[14] ^= -1;
        for (u64 i = 0; i < r; i++){
                const u8 *s = sigma[i % 10];
                G(v,0,4,8,12,m[s[0]],m[s[1]]);
                G(v,1,5,9,13,m[s[2]],m[s[3]]);
                G(v,2,6,10,14,m[s[4]],m[s[5]]);
                G(v,3,7,11,15,m[s[6]],m[s[7]]);

                G(v,0,5,10,15,m[s[8]],m[s[9]]);
                G(v,1,6,11,12,m[s[10]],m[s[11]]);
                G(v,2,7,8,13,m[s[12]],m[s[13]]);
                G(v,3,4,9,14,m[s[14]],m[s[15]]);
        }
        #pragma GCC unroll 8
        for(u64 i = 0; i < 8; i++) (*h)[i] ^= (v[i] ^ v[i+8]);
}

#define ceil(x,y) ((x+y-1)/y)

typedef u64 b[16];
i32 blake2d(u8* out, u64 out_len, u8* in, u64 in_length){
        HT h = {
                0x6A09E667F3BCC908, 
                0xBB67AE8584CAA73B,
                0x3C6EF372FE94F82B,
                0xA54FF53A5F1D36F1,
                0x510E527FADE682D1,
                0x9B05688C2B3E6C1F,
                0x1F83D9ABFB41BD6B,
                0x5BE0CD19137E2179
        };
        h[0] ^= 0x01010000 ^ (0 << 8) ^ out_len;
        u64 dd = ceil(in_length, 128);
        if(dd > 1){
                for(u64 i = 0; i < dd-1; i++){
                        F(&h, (u64*)(in + i*128), (i+1)*128, 0);
                }
        }
       
        u64 last[16] = {0};
        u64 offset = 128*(dd-1);
        u64 remaining = in_length - offset;
        cpy(last, in + offset, remaining);
        F(&h, last, in_length, 1);

        cpy(out,h,out_len>64?64:out_len);
        return 0;
}
