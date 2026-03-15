#ifndef sched_setparam_H
#define sched_setparam_H
#include "../sched_policy.h"
#include "../../../typos.h"
i32 sched_setparam(i32 pid,const enum SchedPolicies* param);
#endif

