#include "lineset.h"
#include "string.h"
#include "../ping/ping.h"
#include "../mem/mem.h"

int main(){
        ping(1);
        str test_string = "key1000=1000\no=1\r\nj=true\rab=FaLsE\r\nfloat:)=11239.1001";
        LineSet ls = {0};
        i32 s = lineset_parse(test_string, strlen(test_string), &ls);
        if(s >= 0){
                ping(2);
                int i = 0;
                LineSetValue value = {
                        .value = (mem)&i,
                        .value_type = String,
                        .size = 0
                };
                lineset_get(&ls, "key1000", &value);
                if(value.value_type == Number && *(i64*)value.value == 1000){
                        ping(100);
                }
                lineset_get(&ls, "o", &value);
                if(value.value_type == Number && *(i64*)value.value == 1){
                        ping(101);
                }
                if(value.value_type != Number || *(i64*)value.value!=1){
                        ping(-101);
                        ping(3);
                        switch (value.value_type){
                                case Number:
                                        ping(1);break;
                                case String:
                                        ping(2);break;
                                case Float:
                                        ping(3);break;
                                case Bool:
                                        ping(4);break;
                        }
                        ping(*(i64*)value.value);
                        ping(-3);
                }
                lineset_get(&ls, "j", &value);
                if(value.value_type == Bool && *(u8*)value.value != 0x00){
                        ping(102);
                }
                lineset_get(&ls, "ab", &value);
                if(value.value_type == Bool && *(u8*)value.value == 0x00){
                        ping(103);
                }
                lineset_get(&ls, "float:)", &value);
                bf64 reference = {0};
                cpy(&reference,value.value,8);
                if(value.value_type == Float && (f64)(reference.num) == 11239.1001){
                        ping(104);
                }else{
                        ping(reference.num);
                }
        }
        ping(-1);
        return 0;
}
