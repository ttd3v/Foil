#ifndef RANDOM_H
#define RANDOM_H
#include "../../typos.h"
i32 random(ref buffer, u64 length);
/// An abstraction over the "random" method, it fills an entire buffer with random derived outputs
i32 randall(ref buffer, u64 length);
/// A version of randall, made to be faster (avoid syscalls), it uses random to stablish a seed, and by doing a simple hash
/// it fills the buffer. This cannot be used when the buffer must be filled with cryptographic data, but when pseudo-random
/// data is enough.
i32 rand_vomit(ref buffer, u64 length);
#endif
