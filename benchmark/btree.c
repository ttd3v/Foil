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

#define TARGET 10000

#define COL_OP   20
#define COL_US   12
#define COL_OPS  14

// right-align val into a fixed-width space-padded field
static i32 pad_u64(u64 val, char* buf, i32 width){
    char tmp[32] = {0};
    i32 l = stringify_u64(val, tmp);
    i32 pad = width - l;
    for(i32 i = 0; i < pad; i++) buf[i] = ' ';
    for(i32 i = 0; i < l;   i++) buf[pad + i] = tmp[i];
    return width;
}

static void print_row(char* op, i32 op_len, u64 micros, u64 throughput){
    char buf[64];

    // operation name — left-aligned, pad right with spaces
    print(op, op_len);
    i32 pad = COL_OP - op_len;
    for(i32 i = 0; i < pad; i++) buf[i] = ' ';
    print(buf, pad);

    // micros — right-aligned
    i32 l = pad_u64(micros, buf, COL_US);
    print(buf, l);

    // throughput — right-aligned
    l = pad_u64(throughput, buf, COL_OPS);
    buf[l] = '\n';
    print(buf, l + 1);
}

i32 main(){
    {
        char header[] =
            "operation           "
            "      micros"
            "     ops/sec\n";
        print(header, sizeof(header) - 1);
        char sep[] =
            "--------------------"
            "------------"
            "--------------\n";
        print(sep, sizeof(sep) - 1);
    }

    u64* kv = map(TARGET * 8);
    randall(kv, TARGET * 8);
    u64* keys = kv;

    i32 priority = 99;
    sched_setscheduler(0, SCHED_FIFO, &priority);

    u32 current_cpu;
    getcpu(&current_cpu, (u32*)-1);
    u8 cpu_map[128] = {0};
    cpu_map[current_cpu / 8] |= (1 << (current_cpu % 8));
    sched_setaffinity(0, 128, cpu_map);

    struct timespec start, end;
    btreeU64 *root;
    btreeU64_new(&root);

    // push
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i32 i = 0; i < TARGET; i++) btreeU64_push(root, keys[i], i);
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        u64 d = (end.tv_nsec - start.tv_nsec) / 1000;
        char op[] = "btreeu64_push";
        print_row(op, sizeof(op) - 1, d, (TARGET * 1000000ULL) / d);
    }

    // get
    clock_gettime(CLOCK_MONOTONIC, &start);
    u64 __value;
    for(i32 i = 0; i < TARGET; i++) btreeU64_get(root, keys[i], &__value);
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        u64 d = (end.tv_nsec - start.tv_nsec) / 1000;
        char op[] = "btreeu64_get";
        print_row(op, sizeof(op) - 1, d, (TARGET * 1000000ULL) / d);
    }

    // remove
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i32 i = 0; i < TARGET; i++) btreeU64_remove(root, keys[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        u64 d = (end.tv_nsec - start.tv_nsec) / 1000;
        char op[] = "btreeu64_remove";
        print_row(op, sizeof(op) - 1, d, (TARGET * 1000000ULL) / d);
    }

    return 0;
}
