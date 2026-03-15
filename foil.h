#ifndef foil_h
#define foil_h
// typos
#include "typos.h"

// constants
#include "constants/errno.h"

// algorithms
#include "algorithms/hash/hash.h"
#include "algorithms/lineset/lineset.h"
#include "algorithms/parsers/parsers.h"
#include "algorithms/stringify/stringify.h"

// atomic
#include "atomic/queue/aqueue.h"

// data-structures
#include "data-structures/array/array.h"
#include "data-structures/btree/btree.h"

// debug
#include "debug/ping/ping.h"
#include "debug/print/print.h"

// syscall
#include "syscall/close/close.h"
#include "syscall/fstat/fstat.h"
#include "syscall/iouring/iouring.h"
#include "syscall/mem/mem.h"
#include "syscall/mkdir/mkdir.h"
#include "syscall/open/open.h"
#include "syscall/read/read.h"
#include "syscall/write/write.h"

// syscall/network
#include "syscall/network/network.h"
#include "syscall/network/accept/accept.h"
#include "syscall/network/bind/bind.h"
#include "syscall/network/connect/connect.h"
#include "syscall/network/listen/listen.h"
#include "syscall/network/setsockopt/setsockopt.h"
#include "syscall/network/socket/socket.h"
#endif
