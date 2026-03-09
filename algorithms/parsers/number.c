#include "parsers.h"

i32 parse_number(str number, u64 length, i64 *output){
        if (length == 0) {*output=0; return 0;};
        i32 endpoint = number[0] == '-' ? -1 : 1;
        i64 current = 0;
        counter i = endpoint < 0?1:0;
        if (endpoint < 0 && length == 1) return -1;

        while (i < length){
                u8 c = *(number+i++);
                if (c < 48 || c > 57){return -1;};
                current *= 10;
                current += c - '0';
        }
        current *= endpoint;
        *output = current;
        return 0;
}
