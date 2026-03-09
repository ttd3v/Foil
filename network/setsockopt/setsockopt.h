#ifndef SETSOCKOPT_H
#define SETSOCKOPT_H
#include "../network.h"
i32 setsockopt(i32 fd, i32 level, i32 optname, const ref optval, u64 optlen);
#endif
