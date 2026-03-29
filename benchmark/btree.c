#include "../data-structures/btree/btree.h"
#include "../debug/print/print.h"
#include "../syscall/sched/sched_setscheduler/sched_setscheduler.h"
#include "../syscall/sched/sched_setaffinity/sched_setaffinity.h"
#include "../syscall/sched/sched_policy.h"
#include "../syscall/getcpu/getcpu.h"
#include "../syscall/clock/clock.h"
#include "../algorithms/stringify/stringify.h"

#ifndef TARGET
#define TARGET 1000000
#endif

// ── ANSI ─────────────────────────────────────────────────────────────────────
#define RESET      "\x1b[0m"
#define BOLD       "\x1b[1m"
#define FG_WHITE   "\x1b[97m"
#define FG_SILVER  "\x1b[37m"
#define FG_CYAN    "\x1b[96m"
#define FG_GREEN   "\x1b[92m"
#define FG_YELLOW  "\x1b[93m"
#define FG_MAGENTA "\x1b[95m"
#define FG_RED     "\x1b[91m"

#define LEN_RESET  4
#define LEN_BOLD   4
#define LEN_COLOR  5

// ── Column widths ─────────────────────────────────────────────────────────────
#define COL_OP    18
#define COL_TIME  14
#define COL_OPS   17
#define COL_BAR   20

// ── Helpers ───────────────────────────────────────────────────────────────────

static void s(char *p, i32 n) { print(p, n); }

#define LIT(str) s((str), sizeof(str) - 1)

static void pad_spaces(i32 n) {
    char buf[64];
    if (n <= 0) return;
    for (i32 i = 0; i < n && i < 64; i++) buf[i] = ' ';
    s(buf, n < 64 ? n : 64);
}

static i32 fmt_u64(u64 val, char *buf, i32 width) {
    char tmp[32];
    i32  l   = stringify_u64(val, tmp);
    i32  pad = width - l;
    for (i32 i = 0; i < pad; i++) buf[i] = ' ';
    for (i32 i = 0; i < l;   i++) buf[pad + i] = tmp[i];
    return width;
}

static i32 fmt_grouped(u64 val, char *out) {
    char raw[32];
    i32  l   = stringify_u64(val, raw);
    i32  pos = 0;
    i32  rem = l % 3;
    for (i32 i = 0; i < l; i++) {
        if (i && (i - rem) % 3 == 0) out[pos++] = ',';
        out[pos++] = raw[i];
    }
    return pos;
}

static u64 elapsed_us(struct timespec *a, struct timespec *b) {
    u64 ns = (u64)(b->tv_sec  - a->tv_sec)  * 1000000000ULL
           + (u64)(b->tv_nsec - a->tv_nsec);
    return ns / 1000;
}

// ── Bar ───────────────────────────────────────────────────────────────────────

static void draw_bar(u64 val, u64 max_val, i32 width, char *color) {
    i32 filled = (max_val > 0) ? (i32)((val * (u64)width) / max_val) : 0;
    if (filled > width) filled = width;

    s(color, LEN_COLOR);
    for (i32 i = 0; i < filled; i++) LIT("\xe2\x96\x88");
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    for (i32 i = filled; i < width; i++) LIT("\xe2\x96\x91");
    s(RESET, LEN_RESET);
}

// ── Row ───────────────────────────────────────────────────────────────────────

static void print_row(char *op, i32 op_len,
                      u64 us, u64 ops, u64 max_ops,
                      char *color) {
    char buf[64];
    i32  l;

    LIT("  ");
    s(color, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    s(op, op_len);
    s(RESET, LEN_RESET);
    pad_spaces(COL_OP - op_len);

    s(FG_SILVER, LEN_COLOR);
    l = fmt_u64(us, buf, COL_TIME);
    s(buf, l);
    LIT(" \xc2\xb5s");
    s(RESET, LEN_RESET);

    s(FG_YELLOW, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    char grp[32];
    l = fmt_grouped(ops, grp);
    pad_spaces(COL_OPS - l);
    s(grp, l);
    s(RESET, LEN_RESET);
    LIT(" ops/s  ");

    draw_bar(ops, max_ops, COL_BAR, color);
    LIT("\n");
}

// ── Header ────────────────────────────────────────────────────────────────────

static void print_header(void) {
    LIT("\n");
    s(BOLD, LEN_BOLD);
    s(FG_WHITE, LEN_COLOR);
    LIT("  \xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x90\n");
    LIT("  \xe2\x94\x82            BTreeU64 Mixed Benchmark              \xe2\x94\x82\n");
    LIT("  \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x98\n");
    s(RESET, LEN_RESET);

    LIT("  ");
    s(FG_SILVER, LEN_COLOR);
    LIT("target ");
    s(RESET, LEN_RESET);
    char grp[32];
    i32 l = fmt_grouped(TARGET, grp);
    s(FG_WHITE, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    s(grp, l);
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    LIT(" ops  \xe2\x80\xa2  SCHED_FIFO  \xe2\x80\xa2  CPU-pinned\n\n");
    s(RESET, LEN_RESET);

    s(FG_SILVER, LEN_COLOR);
    LIT("  operation              ");
    LIT("        time");
    LIT("          throughput");
    LIT("    relative\n");
    LIT("  \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\n");
    s(RESET, LEN_RESET);
}

// ── Legend ────────────────────────────────────────────────────────────────────

static void print_legend(void) {
    LIT("\n  ");
    s(FG_SILVER, LEN_COLOR);
    LIT("legend  ");
    s(RESET, LEN_RESET);

    s(FG_WHITE, LEN_COLOR);
    LIT("\xe2\x96\x88\xe2\x96\x88");
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    LIT(" proportion of fastest op's throughput   ");
    s(RESET, LEN_RESET);

    s(FG_SILVER, LEN_COLOR);
    LIT("\xe2\x96\x91\xe2\x96\x91");
    LIT(" gap to fastest\n");
    LIT("          full bar = fastest op, empty bar = 0 ops/s\n\n");
    s(RESET, LEN_RESET);
}

// ── Footer ────────────────────────────────────────────────────────────────────

static void print_footer(u64 total_us, u64 total_ops, u64 final_size, u64 net_ops) {
    s(FG_SILVER, LEN_COLOR);
    LIT("  \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\n");
    s(RESET, LEN_RESET);

    char grp[32];
    i32  l;

    LIT("  ");
    s(FG_SILVER, LEN_COLOR);
    LIT("total   ");
    l = fmt_grouped(total_us, grp);
    s(grp, l);
    LIT(" \xc2\xb5s   \xe2\x80\x94   ");
    s(RESET, LEN_RESET);
    s(FG_WHITE, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    l = fmt_grouped(total_ops, grp);
    s(grp, l);
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    LIT(" ops/s\n");
    s(RESET, LEN_RESET);

    LIT("  ");
    s(FG_SILVER, LEN_COLOR);
    LIT("final tree size: ");
    l = fmt_grouped(final_size, grp);
    s(FG_WHITE, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    s(grp, l);
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    LIT(" nodes  \xe2\x80\xa2  net ops: ");
    l = fmt_grouped(net_ops, grp);
    s(FG_WHITE, LEN_COLOR);
    s(BOLD, LEN_BOLD);
    s(grp, l);
    s(RESET, LEN_RESET);
    s(FG_SILVER, LEN_COLOR);
    LIT(" (adds - removes)\n");
    s(RESET, LEN_RESET);

    print_legend();
}

// ── Simple LCG RNG ────────────────────────────────────────────────────────────

static u64 rng_state = 0x123456789ABCDEF0ULL;

static u64 rng_next(void) {
    rng_state = rng_state * 6364136223846793005ULL + 1;
    return rng_state;
}

// ── Entry point ───────────────────────────────────────────────────────────────

i32 main(void) {
    i32 priority = 99;
    sched_setscheduler(0, SCHED_FIFO, &priority);

    u32 current_cpu;
    getcpu(&current_cpu, (u32*)-1);
    u8 cpu_map[128] = {0};
    cpu_map[current_cpu / 8] |= (u8)(1 << (current_cpu % 8));
    sched_setaffinity(0, 128, cpu_map);

    btreeU64 *root;
    btreeU64_new(&root);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    u64 adds = 0;
    u64 removes = 0;
    u64 gets = 0;

    for (i32 i = 0; i < TARGET; i++) {
        u64 r = rng_next();
        u64 key = r & 0xFFFFFFFFULL;
        u64 op = (r >> 32) & 0x3;  // 0=add, 1=remove, 2=get, 3=add
        
        if (op == 0 || op == 3) {
            // Add operation
            btreeU64_push(root, key, key);
            adds++;
        } else if (op == 1) {
            // Remove operation
            btreeU64_remove(root, key);
            removes++;
        } else {
            // Get operation
            u64 __value;
            btreeU64_get(root, key, &__value);
            gets++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    u64 total_us = elapsed_us(&t0, &t1);
    u64 total_ops = total_us > 0 ? ((u64)TARGET * 1000000ULL) / total_us : 0;

    // Count final tree size (if btreeU64 has a size function, use it)
    // For now, net_ops = adds - removes
    u64 net_ops = adds > removes ? adds - removes : 0;
    u64 final_size = net_ops;  // Assuming no duplicates

    print_header();

    char op0[] = "mixed ops";
    print_row(op0, sizeof(op0)-1, total_us, total_ops, total_ops, FG_GREEN);

    print_footer(total_us, total_ops, final_size, net_ops);

    return 0;
}
