#ifndef SCHED_RR_GET_INTERVAL_H
#define SCHED_RR_GET_INTERVAL_H
#include "../../../constants/timespec.h"
#include "../../../typos.h"
i32 sched_rr_get_interval(i32 pid, struct timespec*time);
#endif
