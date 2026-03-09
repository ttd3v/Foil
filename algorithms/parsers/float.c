#include "parsers.h"

i32 parse_float(str float_num, u64 length, f64 *output){
    if (length == 0) {*output = 0; return 0;}
    i32 sign = float_num[0] == '-' ? -1 : 1;
    counter i = sign < 0 ? 1 : 0;
    if (sign < 0 && length == 1) return -1;

    f64 integer_part = 0;
    f64 decimal_part = 0;
    f64 decimal_factor = 0.1;
    u8 in_decimal = 0;

    while (i < length){
        u8 c = *(float_num+i++);
        if(c == '.' && !in_decimal){ in_decimal = 1; continue; }
        if(c < 48 || c > 57) return -1;
        if(!in_decimal){
            integer_part = integer_part * 10 + (c - '0');
        } else {
            decimal_part += (c - '0') * decimal_factor;
            decimal_factor *= 0.1;
        }
    }
    *output = sign * (integer_part + decimal_part);
    return 0;
}
