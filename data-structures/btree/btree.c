#include "btree.h"


extern i32 btreeU64_new(btreeU64** output);
extern i32 btreeU64_push(btreeU64* self, u64 key, u64 value);
extern i32 btreeU64_get(btreeU64* self, u64 key, u64 *value);
extern i32 btreeU64_remove(btreeU64* self, u64 key);
extern i32 btreeU64_edit(btreeU64* self, u64 key, u64 value);
