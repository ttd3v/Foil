#ifndef BTREE_H
#define BTREE_H
#include "../../typos.h"
/// BTree that manages u64 pairs (kv)
typedef struct btreeU64 {
        u64 length;
        u64 min;
        u64 max;
        u64 keys[252];
        u64 values[252];
        u64 __padding;
        u64 ___padding;
        struct btreeU64* lnode;
        struct btreeU64* rnode;
        ref parent;
} btreeU64;

_Static_assert(sizeof(btreeU64)==4096,"4096");

/// Initialize a new btree, return <0 on memory allocation failure.
i32 btreeU64_new(btreeU64** output);

/// Inserts a new value into a BTree node 
///
i32 btreeU64_push(btreeU64* self, u64 key, u64 value);

/// Fetch a value from the BTree
/// -1 -> not found
i32 btreeU64_get(btreeU64* self, u64 key, u64 *value);

/// Remove a value from the BTree
/// -1 -> not found
i32 btreeU64_remove(btreeU64* self, u64 key);

#endif
