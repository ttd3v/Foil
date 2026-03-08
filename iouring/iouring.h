#ifndef IOURING_H
#define IOURING_H
#include "../typos.h"

typedef struct {
        u32 head;
        u32 tail;
        u32 ring_mask;
        u32 ring_entries;
        u32 flags;
        u32 dropped;
        u32 array;
        u32 resv[3];
} uring_sq_offsets;

typedef struct {
        u32 head;
        u32 tail;
        u32 ring_mask;
        u32 ring_entries;
        u32 overflow;
        u32 cqes;
        u32 flags;
        u32 resv[3];
} uring_cq_offsets;

typedef struct {
        u8   opcode;
        u8   flags;
        u16  ioprio;
        i32  fd;
        u64  off;
        u64  addr;
        u32  len;
        u32  op_flags;
        u64  user_data;  
        u64  pad[3];
} iou_sqe;

typedef struct {
        u64  user_data;  
        i32  res;
        u32  flags;
} iou_cqe;

typedef struct {
        u32 sq_entries;
        u32 cq_entries;
        u32 flags;
        u32 sq_thread_cpu;
        u32 sq_thread_idle;
        u32 features;
        u32 resv[4];
        uring_sq_offsets sq_off;
        uring_cq_offsets cq_off;
        void* sq_ring;
        void* cq_ring;
        void* sqes;
        i32 fd;
        u32 sq_tail;
        u32 cq_head;
        u32 sq_mask;
        u32 cq_mask;
} uring;

#define IOSQE_FIXED_FILE	(1U << IOSQE_FIXED_FILE_BIT)
#define IOSQE_IO_DRAIN		(1U << IOSQE_IO_DRAIN_BIT)
#define IOSQE_IO_LINK		(1U << IOSQE_IO_LINK_BIT)
#define IOSQE_IO_HARDLINK	(1U << IOSQE_IO_HARDLINK_BIT)
#define IOSQE_ASYNC		(1U << IOSQE_ASYNC_BIT)
#define IOSQE_BUFFER_SELECT	(1U << IOSQE_BUFFER_SELECT_BIT)
#define IOSQE_CQE_SKIP_SUCCESS	(1U << IOSQE_CQE_SKIP_SUCCESS_BIT)
#define IORING_SETUP_IOPOLL	(1U << 0)
#define IORING_SETUP_SQPOLL	(1U << 1)
#define IORING_SETUP_SQ_AFF	(1U << 2)
#define IORING_SETUP_CQSIZE	(1U << 3)
#define IORING_SETUP_CLAMP	(1U << 4)
#define IORING_SETUP_ATTACH_WQ	(1U << 5)
#define IORING_SETUP_R_DISABLED	(1U << 6)
#define IORING_SETUP_SUBMIT_ALL	(1U << 7)
#define IORING_SETUP_COOP_TASKRUN	(1U << 8)
#define IORING_SETUP_TASKRUN_FLAG	(1U << 9)
#define IORING_SETUP_SQE128		(1U << 10)
#define IORING_SETUP_CQE32		(1U << 11)
#define IORING_SETUP_SINGLE_ISSUER	(1U << 12)
#define IORING_SETUP_DEFER_TASKRUN	(1U << 13)
#define IORING_SETUP_NO_MMAP		(1U << 14)
#define IORING_SETUP_REGISTERED_FD_ONLY	(1U << 15)
#define IORING_SETUP_NO_SQARRAY		(1U << 16)
#define IORING_SETUP_HYBRID_IOPOLL	(1U << 17)
#define IORING_SETUP_CQE_MIXED		(1U << 18)
#define IORING_SETUP_SQE_MIXED		(1U << 19)
enum {
	IOSQE_FIXED_FILE_BIT,
	IOSQE_IO_DRAIN_BIT,
	IOSQE_IO_LINK_BIT,
	IOSQE_IO_HARDLINK_BIT,
	IOSQE_ASYNC_BIT,
	IOSQE_BUFFER_SELECT_BIT,
	IOSQE_CQE_SKIP_SUCCESS_BIT,
};
#define IOSQE_FIXED_FILE	(1U << IOSQE_FIXED_FILE_BIT)
#define IOSQE_IO_DRAIN		(1U << IOSQE_IO_DRAIN_BIT)
#define IOSQE_IO_LINK		(1U << IOSQE_IO_LINK_BIT)
#define IOSQE_IO_HARDLINK	(1U << IOSQE_IO_HARDLINK_BIT)
#define IOSQE_ASYNC		(1U << IOSQE_ASYNC_BIT)
#define IOSQE_BUFFER_SELECT	(1U << IOSQE_BUFFER_SELECT_BIT)
#define IOSQE_CQE_SKIP_SUCCESS	(1U << IOSQE_CQE_SKIP_SUCCESS_BIT)


#define IORING_ENTER_GETEVENTS		(1U << 0)
#define IORING_ENTER_SQ_WAKEUP		(1U << 1)
#define IORING_ENTER_SQ_WAIT		(1U << 2)
#define IORING_ENTER_EXT_ARG		(1U << 3)
#define IORING_ENTER_REGISTERED_RING	(1U << 4)

enum io_uring_op {
	IORING_OP_NOP,
	IORING_OP_READV,
	IORING_OP_WRITEV,
	IORING_OP_FSYNC,
	IORING_OP_READ_FIXED,
	IORING_OP_WRITE_FIXED,
	IORING_OP_POLL_ADD,
	IORING_OP_POLL_REMOVE,
	IORING_OP_SYNC_FILE_RANGE,
	IORING_OP_SENDMSG,
	IORING_OP_RECVMSG,
	IORING_OP_TIMEOUT,
	IORING_OP_TIMEOUT_REMOVE,
	IORING_OP_ACCEPT,
	IORING_OP_ASYNC_CANCEL,
	IORING_OP_LINK_TIMEOUT,
	IORING_OP_CONNECT,
	IORING_OP_FALLOCATE,
	IORING_OP_OPENAT,
	IORING_OP_CLOSE,
	IORING_OP_FILES_UPDATE,
	IORING_OP_STATX,
	IORING_OP_READ,
	IORING_OP_WRITE,
	IORING_OP_FADVISE,
	IORING_OP_MADVISE,
	IORING_OP_SEND,
	IORING_OP_RECV,
	IORING_OP_OPENAT2,
	IORING_OP_EPOLL_CTL,
	IORING_OP_SPLICE,
	IORING_OP_PROVIDE_BUFFERS,
	IORING_OP_REMOVE_BUFFERS,
	IORING_OP_TEE,
	IORING_OP_SHUTDOWN,
	IORING_OP_RENAMEAT,
	IORING_OP_UNLINKAT,
	IORING_OP_MKDIRAT,
	IORING_OP_SYMLINKAT,
	IORING_OP_LINKAT,
	IORING_OP_MSG_RING,
	IORING_OP_FSETXATTR,
	IORING_OP_SETXATTR,
	IORING_OP_FGETXATTR,
	IORING_OP_GETXATTR,
	IORING_OP_SOCKET,
	IORING_OP_URING_CMD,
	IORING_OP_SEND_ZC,
	IORING_OP_SENDMSG_ZC,
	IORING_OP_READ_MULTISHOT,
	IORING_OP_WAITID,
	IORING_OP_FUTEX_WAIT,
	IORING_OP_FUTEX_WAKE,
	IORING_OP_FUTEX_WAITV,
	IORING_OP_FIXED_FD_INSTALL,
	IORING_OP_FTRUNCATE,
	IORING_OP_BIND,
	IORING_OP_LISTEN,
	IORING_OP_RECV_ZC,
	IORING_OP_EPOLL_WAIT,
	IORING_OP_READV_FIXED,
	IORING_OP_WRITEV_FIXED,
	IORING_OP_PIPE,
	IORING_OP_NOP128,
	IORING_OP_URING_CMD128,
	IORING_OP_LAST,
};


i32 iou_make(uring* output, u32 entries, u32 iou_flags);
i32 iou_register(uring* ring, u32 opcode, ref arg, u32 nr_args);
i32 iou_paint(uring *ring, u32 opcode, u64 fd, u64 addr, u64 len, u64 offset, u64 userdata, u32 op_flags);
i32 iou_enter(uring *ring, u32 to_submit, u32 min_complete, u64 flags, u64 arg, u64 argz);
i32 iou_spill(uring *ring, iou_cqe *out, u32 max);
#endif
