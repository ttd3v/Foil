#include "../mem/mem.h"
#include "iouring.h"
i32 iou_make(uring *output, u32 entries, u32 iou_flags){
        i32 fd;
        output->flags = iou_flags;
        asm volatile (
                "syscall\n"
                : "=a" (fd)
                : "a" (425), "D" (entries), "S" (output)
                : "rcx", "r11", "memory"
        );
        if (fd < 0) return -1;
        output->sq_ring = mmap(0,output->sq_off.array + output->sq_entries * sizeof(u32), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_POPULATE, fd, 0);
        output->sqes = mmap(0,output->sq_entries * sizeof(iou_sqe),PROT_READ|PROT_WRITE, MAP_SHARED|MAP_POPULATE, fd, 0x10000000ULL);
        output->cq_ring = mmap(0,output->cq_off.cqes + output->cq_entries * sizeof(iou_cqe),PROT_READ|PROT_WRITE, MAP_SHARED|MAP_POPULATE, fd, 0x8000000ULL);
        output->fd = fd;
        output->sq_mask = *(u32*)((u8*)output->sq_ring + output->sq_off.ring_mask);
        output->cq_mask = *(u32*)((u8*)output->cq_ring + output->cq_off.ring_mask);
        return 0;
}
i32 iou_paint(uring *ring, u32 opcode, u64 fd, u64 addr, u64 len, u64 offset, u64 userdata, u32 op_flags){
        iou_sqe *sqe = (iou_sqe*)ring->sqes + (ring->sq_tail & ring->sq_mask);
        sqe->op_flags = op_flags;
        sqe->opcode    = opcode;
        sqe->fd        = fd;
        sqe->addr      = addr;
        sqe->len       = len;
        sqe->off       = offset;
        sqe->user_data = userdata;
        ring->sq_tail++;
        *(u32*)((u8*)ring->sq_ring + ring->sq_off.tail) = ring->sq_tail;
        return 0;
}
i32 iou_enter(uring *ring, u32 to_submit, u32 min_complete, u64 flags, u64 arg, u64 argz){
        i32 output = 0;
        register u64 r10_val asm("r10") = flags;
        register u64 r8_val  asm("r8")  = arg;
        register u64 r9_val  asm("r9")  = argz;
        asm volatile (
                "syscall\n"
                : "=a" (output)
                : "a" (426), "D" (ring->fd), "S" (to_submit), "d" (min_complete), "r" (r10_val), "r" (r8_val), "r" (r9_val)
                : "rcx", "r11", "memory"
        );
        return output;
}

i32 iou_spill(uring *ring, iou_cqe *out, u32 max) {
    u32 tail = *(u32*)((u8*)ring->cq_ring + ring->cq_off.tail);
    u32 count = 0;
    while (ring->cq_head != tail && count < max) {
        iou_cqe *cqe = (iou_cqe*)((u8*)ring->cq_ring + ring->cq_off.cqes) 
                       + (ring->cq_head & ring->cq_mask);
        out[count++] = *cqe;
        ring->cq_head++;
    }
    *(u32*)((u8*)ring->cq_ring + ring->cq_off.head) = ring->cq_head;
    return count;
}


