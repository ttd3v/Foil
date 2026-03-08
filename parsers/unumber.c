#include "parsers.h"

i32 parse_unumber(str unumber, u64 length, u64 *output){
        if(length > 20) return -1;
        i32 current = 0;
        counter i = 0;
        while (i < length){
                u8 c = *(unumber+i++);
                if (c < 48 || c > 57){return -1;};
                current *= 10;
                current += c - '0';
        }
        *output = current;
        return 0;
}
