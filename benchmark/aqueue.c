#include "../atomic/queue/aqueue.h"
#include "../debug/print/print.h"
#include "../syscall/sched/sched_setscheduler/sched_setscheduler.h"
#include "../syscall/sched/sched_setaffinity/sched_setaffinity.h"
#include "../syscall/sched/sched_policy.h"
#include "../syscall/getcpu/getcpu.h"
#include "../syscall/clock/clock.h"
#include "../algorithms/stringify/stringify.h"

// 16384 pages -> 64MB buffer -> 8,388,608 slots
#define PAGES 16384

#define COL_OP  22
#define COL_US  14
#define COL_OPS 16

static u64 delta_micros(struct timespec *a, struct timespec *b){
    u64 ns = (b->tv_sec - a->tv_sec) * 1000000000ULL
           + (b->tv_nsec - a->tv_nsec);
    return ns / 1000;
}

static i32 pad_u64(u64 val, char *buf, i32 width){
    char tmp[32] = {0};
    i32 l = stringify_u64(val, tmp);
    i32 pad = width - l;
    for(i32 i = 0; i < pad; i++) buf[i] = ' ';
    for(i32 i = 0; i < l;   i++) buf[pad + i] = tmp[i];
    return width;
}

static void print_row(char *op, i32 op_len, u64 micros, u64 ops, u64 throughput){
    char buf[64];

    print(op, op_len);
    i32 pad = COL_OP - op_len;
    for(i32 i = 0; i < pad; i++) buf[i] = ' ';
    print(buf, pad);

    i32 l = pad_u64(micros, buf, COL_US);
    print(buf, l);

    l = pad_u64(throughput, buf, COL_OPS);
    buf[l] = '\n';
    print(buf, l + 1);
}

i32 main(){
    {
        char header[] =
            "operation             "
            "        micros"
            "        ops/sec\n";
        print(header, sizeof(header) - 1);
        char sep[] =
            "----------------------"
            "--------------"
            "----------------\n";
        print(sep, sizeof(sep) - 1);
    }

    i32 priority = 99;
    sched_setscheduler(0, SCHED_FIFO, &priority);

    u32 current_cpu;
    getcpu(&current_cpu, (u32*)-1);
    u8 cpu_map[128] = {0};
    cpu_map[current_cpu / 8] |= (1 << (current_cpu % 8));
    sched_setaffinity(0, 128, cpu_map);

    aqueue q;
    aqueue_make(&q, PAGES);
    u64 cap = q.capacity;

    struct timespec start, end;
    u64 value;

    // push — fill the queue completely
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(u64 i = 0; i < cap; i++){
        value = i;
        aqueue_push(&q, &value);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        u64 d = delta_micros(&start, &end);
        char op[] = "aqueue_push";
        print_row(op, sizeof(op) - 1, d, cap, (cap * 1000000ULL) / d);
    }

    // pop — drain the queue completely
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(u64 i = 0; i < cap; i++){
        aqueue_pop(&q, &value);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        u64 d = delta_micros(&start, &end);
        char op[] = "aqueue_pop";
        print_row(op, sizeof(op) - 1, d, cap, (cap * 1000000ULL) / d);
    }

    // interleaved — push one, pop one, steady state, half capacity
    u64 half = cap / 2;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(u64 i = 0; i < half; i++){
        value = i;
        aqueue_push(&q, &value);
        aqueue_pop(&q, &value);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    {
        // each iteration is 2 ops
        u64 d = delta_micros(&start, &end);
        u64 ops = half * 2;
        char op[] = "aqueue_interleaved";
        print_row(op, sizeof(op) - 1, d, ops, (ops * 1000000ULL) / d);
    }

    aqueue_destroy(&q);
    return 0;
}
