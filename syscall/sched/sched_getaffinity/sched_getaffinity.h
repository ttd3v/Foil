#ifndef SCHED_GETAFFINITY_H
#define SCHED_GETAFFINITY_H
#include "../../../typos.h"
i32 sched_getaffinity(i32 pid, u64 cpu_set_size, ref mask);
#endif
