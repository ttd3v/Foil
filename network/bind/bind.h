#ifndef BIND_H
#define BIND_H
#include "../network.h"
i32 bind(i32 fd, struct sockaddr* addr, u64 socklen);
#endif
