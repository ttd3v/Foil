#include "aqueue.h"
#include "../../mem/mem.h"

#define PAGE 4096
#define CELL_SIZE 8

i32 aqueue_make(aqueue *output, u64 buffer_size){
        if(buffer_size == 0){return -2;};
        ref heap = map(PAGE * buffer_size);
        if(heap == MAP_FAIL){
                return -1;
        }
        output->buffer_size = buffer_size*PAGE;
        output->buffer = heap;
        output->capacity = output->buffer_size/CELL_SIZE;
        output->tail = 0;
        output->head = 0;
        return 0;
}
void aqueue_destroy(aqueue *aq){
        unmap(aq->buffer, aq->buffer_size*PAGE);
        aq->buffer=MAP_FAIL;
        return;
}
extern i32 aqueue_push(aqueue* aq, u64* element);
extern i32 aqueue_pop(aqueue* aq, u64* out_element);
