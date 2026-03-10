#include "btree.h"

extern i32 btreeU64_new(btreeU64 **output);
extern void btreeU64_get(btreeU64 *self, u64 key, u64 *value);
extern void btreeU64_push(btreeU64 *self, u64 key, u64 value);
extern void btreeU64_remove(btreeU64 *self, u64 key);
