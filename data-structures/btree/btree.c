#include "btree.h"

extern void btreeU64_new(btreeU64 **output);
extern void btreeU64_get(btreeU64 *self, u64 key, u64 *value);
extern void btreeU64_insert(btreeU64 *self, u64 key, u64 value);
extern void btreeU64_remove(btreeU64 *self, u64 key);
