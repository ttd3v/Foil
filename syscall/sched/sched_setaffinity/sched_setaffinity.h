#ifndef SCHED_SETAFFINITY_H
#define SCHED_SETAFFINITY_H
#include "../../../typos.h"
i32 sched_setaffinity(i32 pid, u64 cpu_set_size, ref mask);
#endif
