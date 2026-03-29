#ifndef BTREE_H
#define BTREE_H
#include "../../typos.h"
/// BTree that manages u64 pairs (kv)
#define CELLS 9
typedef struct  {
        u64 min;
        u64 max;
        u64 length;
        ref branch[(4096-24-CELLS*16)/8];
        u64 keys[CELLS];
        u64 values[CELLS];
} btreeU64;

_Static_assert(sizeof(btreeU64)==4096,"4096");

/// Initialize a new btree, return <0 on memory allocation failure.
/// The pointer is set on *output, the return is 0 for success.
i32 btreeU64_new(btreeU64** output);

/// Inserts a new value into a BTree node 
/// it may allocate more memory for adding new keys, in this case, if it fails
/// the returning value will be <0.
i32 btreeU64_push(btreeU64* self, u64 key, u64 value);

/// Fetch a value from the BTree, if none is found, the return value is -1. 
i32 btreeU64_get(btreeU64* self, u64 key, u64 *value);

/// It query for the key, it removes a value associated with it. If any value is found, it'll be removed
/// from the node and 0 will be returned. If nothing is found, -1 is returned instead.
i32 btreeU64_remove(btreeU64* self, u64 key);

/// It query for the key, if there is a value associated, the value will be mutated and zero will be returned. -1 if the value
/// have not been found.
i32 btreeU64_edit(btreeU64* self, u64 key, u64 value);

#endif
