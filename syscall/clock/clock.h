#ifndef CLOCK_H
#define CLOCK_H
#include "../../typos.h"
#include "../../constants/timespec.h"
i32 clock_settime(i32 clockid, const struct timespec* time);
i32 clock_gettime(i32 clockid,struct timespec *time);
i32 clock_getres(i32 clockid,struct timespec *time);
i32 clock_nanosleep(i32 clockid,i32 flags, struct timespec *request, struct timespec* remain);
#endif
