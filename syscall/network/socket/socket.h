#ifndef SOCKET_H
#define SOCKET_H
#include "../../../typos.h"

/// Initialize a new socket
/// socket() creates an endpoint for communication and returns a file descriptor. The socket is
/// not yet connected and has no local address bound. It must be subsequently used with bind(),
/// connect() , listen() , accept() , etc.
i32 socket(i32 addr_family, i32 socket_type, i32 protocol);

#endif
