#include "stringify.h"

i32 stringify_u64(u64 input, str output){
    if(!output) return -1;

    if(input == 0){
        output[0] = '0';
        output[1] = '\0';
        return 1;
    }

    char tmp[32];
    i32 ti = 0;

    while(input != 0 && ti < 32){
        tmp[ti++] = (char)('0' + (input % 10));
        input /= 10;
    }

    if(ti >= 32) return -2;

    for(i32 i = 0; i < ti; i++){
        output[i] = tmp[ti - 1 - i];
    }
    output[ti] = '\0';
    return ti;
}

i32 stringify_i64(i64 input, str output){
    if(!output) return -1;

    if(input < 0){
        u64 absval = (u64)(-(input + 1)) + 1ULL;
        i32 written = stringify_u64(absval, output + 1);
        if(written < 0) return written;
        output[0] = '-';
        return 1 + written;
    }else{
        return stringify_u64((u64)input, output);
    }
}

i32 stringify_f64(f64 input, str output){
    if(!output) return -1;

    if(input == 0.0){
        output[0] = '0';
        output[1] = '\0';
        return 1;
    }

    i32 cursor = 0;

    if(input < 0.0){
        output[cursor++] = '-';
        input = -input;
    }

    u64 intpart = (u64)input;
    f64 fracpart = input - (f64)intpart;

    char intbuf[32];
    i32 ilen = stringify_u64(intpart, intbuf);
    if(ilen < 0) return -2;

    for(i32 i = 0; i < ilen; i++)
        output[cursor++] = intbuf[i];

    if(fracpart > 0.0){
        output[cursor++] = '.';
        for(i32 i = 0; i < 16 && fracpart > 0.0; i++){
            fracpart *= 10.0;
            u64 digit = (u64)fracpart;
            output[cursor++] = (char)('0' + digit);
            fracpart -= (f64)digit;
        }
    }

    output[cursor] = '\0';
    return cursor;
}

