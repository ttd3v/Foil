#ifndef BTREE_H
#define BTREE_H
#include "../../typos.h"
/// BTree that manages u64 pairs (kv)
typedef struct btreeU64 {
        u64 min;
        u64 lnode_min;
        u64 rnode_min;
        u64 max;
        u64 lnode_max;
        u64 rnode_max;
        u64 length;
        u64 keys[251];
        u64 values[251];
        struct btreeU64* lnode;
        struct btreeU64* rnode;
        u64 __padding;
} btreeU64;

_Static_assert(sizeof(btreeU64)==4096,"4096");

/// Initialize a new btree, return <0 on memory allocation failure.
void btreeU64_new(btreeU64** output);

/// Inserts a new value into a BTree node, if there is a value with the given key already it'll be updated instead. 
void btreeU64_insert(btreeU64* self, u64 key, u64 value);

/// Fetch a value from the BTree
void btreeU64_get(btreeU64* self, u64 key, u64 *value);

/// Remove a value from the BTree
void btreeU64_remove(btreeU64* self, u64 key);

#endif
