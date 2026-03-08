#include "aqueue.h"
#include "../../ping/ping.h"

#define CAP 512

i32 main(){
        u64 vals[CAP];
        u64 out[CAP];

        for(u64 i = 0; i < CAP; i++) vals[i] = i;

        ping(0);
        aqueue obj = {0};
        aqueue_make(&obj, 1);
        ping(1);

        for(u64 i = 0; i < CAP; i++) aqueue_push(&obj, &vals[i]);

        ping(2);
        for(u64 i = 0; i < CAP; i++) out[i] = 0xDEADBEEF;
        ping(3);

        for(u64 i = 0; i < CAP; i++) aqueue_pop(&obj, &out[i]);

        ping(4);
        u64 ok = 1;
        for(u64 i = 0; i < CAP; i++) if(out[i] != i){ ok = 0; break; }
        if(ok) ping(-10);
        ping(10);
        return 0;
}
