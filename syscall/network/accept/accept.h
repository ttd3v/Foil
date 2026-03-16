#ifndef ACCEPT_H
#define ACCEPT_H
#include "../network.h"
i32 accept(i32 sockfd, struct sockaddr *addr, u64 *addrlen);
#endif
