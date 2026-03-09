#ifndef PARSERS_H
#define PARSERS_H
#include "../../typos.h"
/// parse_number
/// It parses a signed integer, it does not care about over/underflows.
///
/// outputs:
/// 0 - Success
/// -1 - Invalid string
/// -2 - Something wrong happened
i32 parse_number(str number, u64 length, i64* output);

/// parse_unumber
/// It parses an unsigned integer, it does not care about overflows.
///
/// outputs:
/// 0 - Success
/// -1 - Invalid string
/// -2 - Something wrong happened
i32 parse_unumber(str unumber, u64 length, u64* output);

/// parse_hex
/// It parses a hex code, it does not care about segfaults
///
/// outputs:
/// 0 - Success
/// -1 - Invalid string
/// -2 - Something wrong happened
// - may not be needed - i32 parse_hex(str hex, u64 length, u8* output);

/// parse_float
/// It parses a floating point number, it does not care about correctness.
///
/// outputs:
/// 0 - Success
/// -1 - Invalid string
/// -2 - Something wrong happened
i32 parse_float(str float_num, u64 length, f64* output);
#endif
