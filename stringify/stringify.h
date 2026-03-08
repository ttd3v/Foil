#ifndef STRINGIFY_H
#define STRINGIFY_H
#include "../../typos.h"

/// Turns an i64 into a string. Its assumed that there are 32 bytes in the output
i32 stringify_i64(i64 input, str output);
/// Turns an u64 into a string. Its assumed that there are 32 bytes in the output
i32 stringify_u64(u64 input, str output);
/// Turns an f64 into a string. Its assumed that there are 64 bytes in the output
i32 stringify_f64(f64 input, str output);

#endif
