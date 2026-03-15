#include "../data-structures/btree/btree.h"
#include "../debug/print/print.h"
#include "../syscall/random/random.h"
#include "../syscall/mem/mem.h"
#include "../syscall/sched/sched_setscheduler/sched_setscheduler.h"
#include "../syscall/sched/sched_setaffinity/sched_setaffinity.h"
#include "../syscall/sched/sched_policy.h"
#include "../syscall/getcpu/getcpu.h"
#include "../syscall/clock/clock.h"
#include "../algorithms/stringify/stringify.h"

#define TARGET 1000000

i32 main(){
        {
                char label[] = "\noperation\tmicros\tthroughput\n";
                print(label,sizeof(label));
        }
        u64* kv = map(TARGET*8);
        randall(kv, TARGET*8);
        u64* keys = kv;
        i32 priority = 99;
        sched_setscheduler(0, SCHED_FIFO, &priority);
        u32 current_cpu;
        getcpu(&current_cpu, (u32*)-1);
        u8 cpu_map[128] = {0};
        cpu_map[0] |= 1;
        sched_setaffinity(0, 1, &cpu_map);
        struct timespec start;
        struct timespec end;
        btreeU64 *root;
        btreeU64_new(&root);
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (i32 i = 0; i < TARGET;i++) btreeU64_push(root, keys[i], i);
        clock_gettime(CLOCK_MONOTONIC, &end);
        u64 d_micro = (end.tv_nsec - start.tv_nsec)/1000;

        {
                char label0[] = "btreeu64_push\t";
                print(label0,sizeof(label0));
                char num[32]={0};
                i32 l = stringify_u64(d_micro, num);
                num[l] = '\t';
                print(num,l);
                u64 throughput = TARGET / d_micro;
                l = stringify_u64(throughput, num);
                num[l] = '\n';
                print(num,l);
        }

        {
        clock_gettime(CLOCK_MONOTONIC, &start);
        u64 __value;
        for (i32 i = 0; i < TARGET;i++) btreeU64_get(root, keys[i], &__value);
        clock_gettime(CLOCK_MONOTONIC, &end);
        d_micro = (end.tv_nsec - start.tv_nsec)/1000;

        {
                char label0[] = "btreeu64_get\t";
                print(label0,sizeof(label0));
                char num[32]={0};
                i32 l = stringify_u64(d_micro, num);
                num[l] = '\t';
                print(num,l);
                u64 throughput = TARGET / d_micro;
                l = stringify_u64(throughput, num);
                num[l] = '\n';
                print(num,l);
        }

        }

        
        {
        clock_gettime(CLOCK_MONOTONIC, &start);
        u64 __value;
        for (i32 i = 0; i < TARGET;i++) btreeU64_remove(root, keys[i]);
        clock_gettime(CLOCK_MONOTONIC, &end);
        d_micro = (end.tv_nsec - start.tv_nsec)/1000;

        {
                char label0[] = "btreeu64_remove\t";
                print(label0,sizeof(label0));
                char num[32]={0};
                i32 l = stringify_u64(d_micro, num);
                num[l] = '\t';
                print(num,l);
                u64 throughput = TARGET / d_micro;
                l = stringify_u64(throughput, num);
                num[l] = '\n';
                print(num,l);
        }

        }
        return 0;
}
