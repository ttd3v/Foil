#include "iouring.h"
#include "../ping/ping.h"

#define BUF_SIZE 4096

i32 main(){
        uring ring = {0};
        ping(0);
        
        if(iou_make(&ring, 8, 0) < 0){ ping(-1); return -1; }
        ping(1);
        iou_paint(&ring, IORING_OP_NOP, 0, 0, 0, 0, 42,0);
        if(iou_enter(&ring, 1, 1, 0, 0, 0) < 0){ ping(-2); return -1; }
        iou_cqe cqes[8];
        i32 count = iou_spill(&ring, cqes, 8);
        if(count != 1){ ping(-3); return -1; }
        if(cqes[0].res != 0){ ping(-4); return -1; }
        if(cqes[0].user_data != 42){ ping(-5); return -1; }
        ping(2); // nop passed
        return 0;
}
