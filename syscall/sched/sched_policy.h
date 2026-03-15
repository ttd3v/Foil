#ifndef SCHED_POLICIES_H 
#define SCHED_POLICIES_H
#include "../../typos.h"
enum SchedPolicies:u32{
        SCHED_NORMAL	=	0, 
        SCHED_FIFO	=	1,
        SCHED_RR	=	2,
        SCHED_BATCH	=	3
};
#endif
