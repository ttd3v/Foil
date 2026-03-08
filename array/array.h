#ifndef ARRAY_H
#define ARRAY_H
#include "../typos.h"
typedef struct{
        ref buffer;
        u64 length;
        u64 pages;
} array;

_Static_assert(sizeof(array) == 24,"Array got an unexpected size");

/// Initializes an array, the "pages" argument defines the count of pages to be allocated upfront. Min:1
///
/// Outputs:
/// -1 -> Failed to allocate memory
/// 0 -> Ok
i32 array_new(array* self, u64 pages);

/// Pushes N items. If the length after the push will be eq capacity, another page will be allocated priorly.
i32 array_push(array* self, ref* element, u64 element_size, u64 N);

/// Swap-delete the element at "u64 position"
/// Removed element is cpy into "ref*element"
i32 array_remove(array* self, ref* element, u64 element_size, u64 position);

/// Pop away the last element.
/// The removed element is cpy into "ref*element"
i32 array_pop(array*self, ref*element, u64 element_size);

/// Read the element at the position
i32 array_read(array*self, ref*element, u64 element_size, u64 position);

/// Allocates the count of pages
i32 array_expand(array*self,u64 count);

/// Shrink the count of pages
i32 array_shrink(array*self,u64 count);

/// Unmap all pages
void array_destroy(array*self);
#endif
