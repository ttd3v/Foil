#ifndef AQUEUE_H
#define AQUEUE_H
#include "../../../typos.h"
typedef struct {
        u64 tail;
        u64 head;
        u64* buffer;
        u64 buffer_size;
        u64 capacity;
} aqueue;
/// Makes a new atomic queue into a pointer
/// u64 cell_size -> size of each element
/// u64 buffer_size -> memory pages to be allocated (4096*N);
/// the capacity of the queue is buffer_size/cell_size
///
/// Output:
///     0 -> Success
///     -1 -> Failed to allocate memory
///     -2 -> invalid buffer_size
i32 aqueue_make(aqueue* output, u64 buffer_size);

/// Pushes a new element into an atomic queue
///
/// Output:
///     0 -> Success
///     -1 -> AQueue is full
i32 aqueue_push(aqueue* aq, u64* element); 
/// Pops an element from an atomic queue, writing into "element"
/// Output:
///     0 -> Success
///     -1 -> AQueue is empty
i32 aqueue_pop(aqueue* aq, u64* element);

/// Frees memory and set aq->buffer MAP_FAIL.
void aqueue_destroy(aqueue* aq);
#endif
