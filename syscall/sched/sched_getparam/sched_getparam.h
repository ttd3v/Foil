#ifndef sched_getparam_H
#define sched_getparam_H
#include "../sched_policy.h"
#include "../../../typos.h"
i32 sched_getparam(i32 pid, enum SchedPolicies* param);
#endif
